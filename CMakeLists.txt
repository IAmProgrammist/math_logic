cmake_minimum_required(VERSION 3.16)
project(math_logic C CXX)

set(CMAKE_CXX_STANDARD 11)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)

install(TARGETS alg DESTINATION .)