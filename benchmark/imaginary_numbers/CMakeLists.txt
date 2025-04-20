
find_package(benchmark REQUIRED)
add_executable(imaginary_numbers imaginary_numbers.cpp)
target_link_libraries(imaginary_numbers PRIVATE rflect::rflect benchmark::benchmark_main)
