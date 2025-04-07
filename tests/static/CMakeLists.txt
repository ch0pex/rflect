
add_library(unit_test_static STATIC
        test_vector_concepts.cpp
        test_proxy.cpp
)

target_link_libraries(unit_test_static PRIVATE rflect::rflect)
