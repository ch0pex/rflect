cmake_minimum_required(VERSION 3.28)

project(Reflection LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src)
include_directories(src/common)

add_compile_options(
        -std=c++26
        -nostdinc++
        -L/usr/lib/llvm-18/lib
        -I/home/chope/reflection/install/include/c++/v1/
        -I/home/chope/reflection/install/include/x86_64-unknown-linux-gnu/c++/v1/
        -I/usr/lib/llvm-18/lib/clang/18/include
        -freflection-latest
        -fexpansion-statements
        -fno-access-contexts
        -Wall
        -Wextra
)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O2 -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -O3 -ffast-math -ftree-vectorize")

link_directories(/usr/lib/llvm-18/lib)
link_libraries(c++ c++abi)

add_subdirectory(src)

add_subdirectory(tests)
