cmake_minimum_required(VERSION 3.20)

project(cover_curve
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(cover_curve
    math/src/numerical/integration.cpp
    math/src/numerical/minimization.cpp
    math/src/numerical/support_max.cpp
    math/src/interpolation/linear.cpp
    math/src/interpolation/natural_cubic_spline.cpp
    math/src/algorithms/adaptive_grid_dp/one_segment_cost.cpp
    math/src/algorithms/adaptive_grid_dp/grid_dp.cpp
    math/src/algorithms/adaptive_grid_dp/solver.cpp
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
