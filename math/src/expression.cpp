#include <cover_curve/expression.hpp>

#include <cmath>
#include <cctype>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace cover_curve {
namespace {

class Parser {
    enum class Kind {
        Constant,
        Variable,
        Add,
        Subtract,
        Multiply,
        Divide,
        Power,
        Negate,
        Function
    };

    using UnaryFunction = double (*)(double);

    struct Node {
        Kind kind;
        double constant = 0.0;
        UnaryFunction function = nullptr;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(Kind k) : kind(k) {}
    };

public:
    explicit Parser(std::string source) : source_(std::move(source)) {}

    Function parse() {
        if (source_.empty())
            throw std::invalid_argument("Enter a function expression.");

        pos_ = 0;
        auto root = parseAdditive();
        skipSpaces();

        if (pos_ != source_.size())
            throw std::invalid_argument(
                "Unexpected token at position " +
                std::to_string(pos_) + "."
            );

        auto sharedRoot =
            std::shared_ptr<const Node>(std::move(root));

        return [root = std::move(sharedRoot)](double x) {
            const double value = evaluate(*root, x);
            if (!std::isfinite(value))
                throw std::invalid_argument(
                    "Function returned a non-finite value."
                );
            return value;
        };
    }

private:
    static double evaluate(const Node& node, double x) {
        switch (node.kind) {
        case Kind::Constant:
            return node.constant;
        case Kind::Variable:
            return x;
        case Kind::Add:
            return evaluate(*node.left, x) + evaluate(*node.right, x);
        case Kind::Subtract:
            return evaluate(*node.left, x) - evaluate(*node.right, x);
        case Kind::Multiply:
            return evaluate(*node.left, x) * evaluate(*node.right, x);
        case Kind::Divide:
            return evaluate(*node.left, x) / evaluate(*node.right, x);
        case Kind::Power:
            return std::pow(
                evaluate(*node.left, x),
                evaluate(*node.right, x)
            );
        case Kind::Negate:
            return -evaluate(*node.left, x);
        case Kind::Function: {
            const double value = node.function(evaluate(*node.left, x));
            if (!std::isfinite(value))
                throw std::invalid_argument(
                    "Function returned a non-finite value."
                );
            return value;
        }
        }

        throw std::logic_error("Invalid expression node.");
    }

    std::unique_ptr<Node> parseAdditive() {
        auto value = parseMultiplicative();

        for (;;) {
            if (consume('+')) {
                value = makeBinary(
                    Kind::Add, std::move(value), parseMultiplicative()
                );
            } else if (consume('-')) {
                value = makeBinary(
                    Kind::Subtract, std::move(value), parseMultiplicative()
                );
            } else {
                return value;
            }
        }
    }

    std::unique_ptr<Node> parseMultiplicative() {
        auto value = parsePower();

        for (;;) {
            if (consume('*')) {
                value = makeBinary(
                    Kind::Multiply, std::move(value), parsePower()
                );
            } else if (consume('/')) {
                value = makeBinary(
                    Kind::Divide, std::move(value), parsePower()
                );
            } else {
                return value;
            }
        }
    }

    std::unique_ptr<Node> parsePower() {
        auto value = parseUnary();
        if (consume('^')) {
            value = makeBinary(
                Kind::Power, std::move(value), parsePower()
            );
        }
        return value;
    }

    std::unique_ptr<Node> parseUnary() {
        if (consume('+'))
            return parseUnary();

        if (consume('-')) {
            auto node = std::make_unique<Node>(Kind::Negate);
            node->left = parseUnary();
            return node;
        }

        return parsePrimary();
    }

    std::unique_ptr<Node> parsePrimary() {
        if (consume('(')) {
            auto value = parseAdditive();
            expect(')');
            return value;
        }

        if (pos_ < source_.size() &&
            (std::isdigit(static_cast<unsigned char>(source_[pos_])) ||
             source_[pos_] == '.')) {
            return makeConstant(parseNumber());
        }

        if (pos_ < source_.size() &&
            (std::isalpha(static_cast<unsigned char>(source_[pos_])) ||
             source_[pos_] == '_')) {
            const std::string name = parseIdentifier();

            if (name == "x")
                return std::make_unique<Node>(Kind::Variable);

            if (name == "pi")
                return makeConstant(std::acos(-1.0));

            if (name == "e")
                return makeConstant(std::exp(1.0));

            const auto fn = function(name);
            if (!fn)
                throw std::invalid_argument(
                    "Unknown identifier '" + name + "'."
                );

            expect('(');

            auto argument = parseAdditive();
            expect(')');

            auto node = std::make_unique<Node>(Kind::Function);
            node->function = fn;
            node->left = std::move(argument);
            return node;
        }

        throw std::invalid_argument(
            "Unexpected token at position " +
            std::to_string(pos_) + "."
        );
    }

    double parseNumber() {
        const std::size_t start = pos_;
        bool digits = false;

        while (pos_ < source_.size() &&
               std::isdigit(
                   static_cast<unsigned char>(source_[pos_])
               )) {
            ++pos_;
            digits = true;
        }

        if (pos_ < source_.size() && source_[pos_] == '.') {
            ++pos_;

            while (pos_ < source_.size() &&
                   std::isdigit(
                       static_cast<unsigned char>(source_[pos_])
                   )) {
                ++pos_;
                digits = true;
            }
        }

        if (!digits)
            throw std::invalid_argument(
                "Expected a number at position " +
                std::to_string(start) + "."
            );

        if (pos_ < source_.size() &&
            (source_[pos_] == 'e' || source_[pos_] == 'E')) {
            ++pos_;

            if (pos_ < source_.size() &&
                (source_[pos_] == '+' || source_[pos_] == '-')) {
                ++pos_;
            }

            const std::size_t exponent = pos_;

            while (pos_ < source_.size() &&
                   std::isdigit(
                       static_cast<unsigned char>(source_[pos_])
                   )) {
                ++pos_;
            }

            if (pos_ == exponent)
                throw std::invalid_argument(
                    "Invalid scientific notation."
                );
        }

        return std::stod(source_.substr(start, pos_ - start));
    }

    std::string parseIdentifier() {
        const std::size_t start = pos_;

        while (pos_ < source_.size() &&
               (std::isalnum(
                    static_cast<unsigned char>(source_[pos_])
                ) ||
                source_[pos_] == '_')) {
            ++pos_;
        }

        return source_.substr(start, pos_ - start);
    }

    static UnaryFunction function(const std::string& name) {
        if (name == "sin") return std::sin;
        if (name == "cos") return std::cos;
        if (name == "tan") return std::tan;
        if (name == "asin") return std::asin;
        if (name == "acos") return std::acos;
        if (name == "atan") return std::atan;
        if (name == "exp") return std::exp;
        if (name == "log") return std::log;
        if (name == "sqrt") return std::sqrt;
        if (name == "abs") return std::fabs;
        if (name == "floor") return std::floor;
        if (name == "ceil") return std::ceil;
        return nullptr;
    }

    static std::unique_ptr<Node> makeConstant(double value) {
        auto node = std::make_unique<Node>(Kind::Constant);
        node->constant = value;
        return node;
    }

    static std::unique_ptr<Node> makeBinary(
        Kind kind,
        std::unique_ptr<Node> left,
        std::unique_ptr<Node> right
    ) {
        auto node = std::make_unique<Node>(kind);
        node->left = std::move(left);
        node->right = std::move(right);
        return node;
    }

    bool consume(char c) {
        skipSpaces();

        if (pos_ < source_.size() && source_[pos_] == c) {
            ++pos_;
            return true;
        }

        return false;
    }

    void expect(char c) {
        if (!consume(c))
            throw std::invalid_argument(
                "Expected '" + std::string(1, c) +
                "' at position " + std::to_string(pos_) + "."
            );
    }

    void skipSpaces() {
        while (pos_ < source_.size() &&
               std::isspace(
                   static_cast<unsigned char>(source_[pos_])
               )) {
            ++pos_;
        }
    }

    std::string source_;
    std::size_t pos_ = 0;
};

}

Function parseExpression(const std::string& expression) {
    return Parser(expression).parse();
}

}
