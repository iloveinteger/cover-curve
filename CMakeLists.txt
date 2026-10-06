cmake_minimum_required(VERSION 3.20)

project(cover_curve
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(cover_curve
    math/src/expression.cpp
    math/src/numerical/integration.cpp
    math/src/numerical/minimization.cpp
    math/src/numerical/support_max.cpp
    math/src/interpolation/linear.cpp
    math/src/interpolation/natural_cubic_spline.cpp
    math/src/algorithms/adaptive_grid_dp/one_segment_cost.cpp
    math/src/algorithms/adaptive_grid_dp/grid_dp.cpp
    math/src/algorithms/adaptive_grid_dp/solver.cpp
    math/src/algorithms/fast_grid_dp/grid_dp.cpp
    math/src/algorithms/fast_grid_dp/solver.cpp
    math/src/algorithms/curvature_adaptive/curvature_grid.cpp
    math/src/algorithms/curvature_adaptive/grid_dp.cpp
    math/src/algorithms/curvature_adaptive/solver.cpp
    math/src/algorithms/breakpoint_search/solver.cpp
)

target_include_directories(cover_curve
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/math/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/math/src
)

if(MSVC)
    target_compile_options(cover_curve PRIVATE /W4)
else()
    target_compile_options(cover_curve PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(cover_curve_smoke
    tests/smoke.cpp
)

target_link_libraries(cover_curve_smoke PRIVATE cover_curve)

enable_testing()
add_test(NAME cover_curve_smoke COMMAND cover_curve_smoke)

if(EMSCRIPTEN)
    add_executable(cover_curve_web
        web/bindings.cpp
    )

    target_link_libraries(cover_curve_web PRIVATE cover_curve)

    target_link_options(cover_curve_web PRIVATE
        "-sMODULARIZE=1"
        "-sEXPORT_NAME=CoverCurve"
        "-sENVIRONMENT=web"
        "-sALLOW_MEMORY_GROWTH=1"
        "--bind"
    )

    set_target_properties(cover_curve_web PROPERTIES
        OUTPUT_NAME "cover_curve"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/web"
    )
endif()


add_executable(cover_curve_benchmark
    tests/benchmark.cpp
)

target_link_libraries(cover_curve_benchmark PRIVATE cover_curve)
