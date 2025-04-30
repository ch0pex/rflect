
find_package(benchmark REQUIRED)
add_executable(imaginary_numbers imaginary_numbers.cpp)
target_link_libraries(imaginary_numbers PRIVATE benchmark::benchmark_main rflect::rflect)
