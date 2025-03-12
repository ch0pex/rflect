cmake_minimum_required(VERSION 3.28)

project(Reflection LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Non-reflected-soa soa/main.cpp)

target_compile_options(Non-reflected-soa PRIVATE
        -std=c++26
        -nostdinc++
        -I/home/chope/reflection/install/include/c++/v1/
        -I/home/chope/reflection/install/include/x86_64-unknown-linux-gnu/c++/v1/
        -I/usr/lib/llvm-18/lib/clang/18/include
        -freflection-latest
)

target_compile_options(Non-reflected-soa PRIVATE -stdlib=libc++)
target_link_directories(Non-reflected-soa PRIVATE /usr/lib/llvm-18/lib)
target_link_libraries(Non-reflected-soa PRIVATE c++ c++abi)
