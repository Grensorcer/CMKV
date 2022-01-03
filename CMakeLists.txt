cmake_minimum_required(VERSION 3.10)

project(EpiTravex VERSION 0.1 LANGUAGES CXX)

add_executable(epitravex main.cc solvers.cc board.cc)
target_compile_features(epitravex PUBLIC cxx_std_17)
target_compile_options(epitravex PUBLIC -Wall -Wextra -pedantic -march=native)
target_include_directories(epitravex PUBLIC
                          "${PROJECT_SOURCE_DIR}/include")
# target_link_libraries(epitravex)

add_executable(bench utils/benchmark.cc solvers.cc board.cc)
target_compile_features(bench PUBLIC cxx_std_17)
target_compile_options(bench PUBLIC -Wall -Wextra -pedantic -march=native)
target_include_directories(bench PUBLIC
                          "${PROJECT_SOURCE_DIR}/include")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "$CMAKE_CXX_FLAGS_DEBUG -DMYDEBUG")

