cmake_minimum_required(VERSION 3.28)

project(Reflection LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(.)

add_compile_options(
        -std=c++26
        -nostdinc++
        -I/home/chope/reflection/install/include/c++/v1/
        -I/home/chope/reflection/install/include/x86_64-unknown-linux-gnu/c++/v1/
        -I/usr/lib/llvm-18/lib/clang/18/include
        -freflection-latest
        -fexpansion-statements
        -fno-access-contexts
)

link_directories(/usr/lib/llvm-18/lib)
link_libraries(c++ c++abi)

add_subdirectory(non-reflected)
add_subdirectory(reflected)

enable_testing()
add_subdirectory(tests)
