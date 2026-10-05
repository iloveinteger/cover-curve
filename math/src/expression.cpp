#include <cover_curve/expression.hpp>

#include <cmath>
#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>

namespace cover_curve {
namespace {

class Parser {
public:
    explicit Parser(std::string source) : source_(std::move(source)) {}

    Function parse() {
        if (source_.empty())
            throw std::invalid_argument("Enter a function expression.");

        return [parser = *this](double x) mutable {
            parser.pos_ = 0;
            parser.currentX_ = x;
            const double value = parser.parseAdditive();
            parser.skipSpaces();
            if (parser.pos_ != parser.source_.size())
                throw std::invalid_argument("Unexpected token at position " +
                                            std::to_string(parser.pos_) + ".");
            if (!std::isfinite(value))
                throw std::invalid_argument("Function returned a non-finite value.");
            return value;
        };
    }

private:
    double parseAdditive() {
        double value = parseMultiplicative();
        for (;;) {
            if (consume('+')) value += parseMultiplicative();
            else if (consume('-')) value -= parseMultiplicative();
            else return value;
        }
    }

    double parseMultiplicative() {
        double value = parsePower();
        for (;;) {
            if (consume('*')) value *= parsePower();
            else if (consume('/')) value /= parsePower();
            else return value;
        }
    }

    double parsePower() {
        double value = parseUnary();
        if (consume('^')) value = std::pow(value, parsePower());
        return value;
    }

    double parseUnary() {
        if (consume('+')) return parseUnary();
        if (consume('-')) return -parseUnary();
        return parsePrimary();
    }

    double parsePrimary() {
        if (consume('(')) {
            double value = parseAdditive();
            expect(')');
            return value;
        }

        if (pos_ < source_.size() &&
            (std::isdigit(static_cast<unsigned char>(source_[pos_])) ||
             source_[pos_] == '.'))
            return parseNumber();

        if (pos_ < source_.size() &&
            (std::isalpha(static_cast<unsigned char>(source_[pos_])) ||
             source_[pos_] == '_')) {
            const std::string name = parseIdentifier();
            if (name == "x") return currentX_;
            if (name == "pi") return std::acos(-1.0);
            if (name == "e") return std::exp(1.0);

            const auto fn = function(name);
            if (!fn) throw std::invalid_argument("Unknown identifier '" + name + "'.");
            expect('(');
            const double value = fn(parseAdditive());
            expect(')');
            if (!std::isfinite(value))
                throw std::invalid_argument("Function '" + name + "' returned a non-finite value.");
            return value;
        }

        throw std::invalid_argument("Unexpected token at position " +
                                    std::to_string(pos_) + ".");
    }

    double parseNumber() {
        const std::size_t start = pos_;
        bool digits = false;
        while (pos_ < source_.size() && std::isdigit(static_cast<unsigned char>(source_[pos_]))) {
            ++pos_; digits = true;
        }
        if (pos_ < source_.size() && source_[pos_] == '.') {
            ++pos_;
            while (pos_ < source_.size() && std::isdigit(static_cast<unsigned char>(source_[pos_]))) {
                ++pos_; digits = true;
            }
        }
        if (!digits)
            throw std::invalid_argument("Expected a number at position " + std::to_string(start) + ".");
        if (pos_ < source_.size() && (source_[pos_] == 'e' || source_[pos_] == 'E')) {
            ++pos_;
            if (pos_ < source_.size() && (source_[pos_] == '+' || source_[pos_] == '-')) ++pos_;
            const std::size_t exponent = pos_;
            while (pos_ < source_.size() && std::isdigit(static_cast<unsigned char>(source_[pos_]))) ++pos_;
            if (pos_ == exponent) throw std::invalid_argument("Invalid scientific notation.");
        }
        return std::stod(source_.substr(start, pos_ - start));
    }

    std::string parseIdentifier() {
        const std::size_t start = pos_;
        while (pos_ < source_.size() &&
               (std::isalnum(static_cast<unsigned char>(source_[pos_])) || source_[pos_] == '_'))
            ++pos_;
        return source_.substr(start, pos_ - start);
    }

    using UnaryFunction = double (*)(double);
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
            throw std::invalid_argument("Expected '" + std::string(1, c) +
                                        "' at position " + std::to_string(pos_) + ".");
    }

    void skipSpaces() {
        while (pos_ < source_.size() &&
               std::isspace(static_cast<unsigned char>(source_[pos_]))) ++pos_;
    }

    std::string source_;
    std::size_t pos_ = 0;
    double currentX_ = 0.0;
};

}

Function parseExpression(const std::string& expression) {
    return Parser(expression).parse();
}

}
