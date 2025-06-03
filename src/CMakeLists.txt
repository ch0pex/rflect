add_library(rflect INTERFACE)

target_sources(
  rflect
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         ${CMAKE_CURRENT_SOURCE_DIR}/include
         FILES
         # Core
         include/rflect/rflect.hpp
         include/rflect/concepts.hpp
         include/rflect/containers.hpp
         include/rflect/converters.hpp
         # Concepts
         include/rflect/concepts/layout_concepts.hpp
         include/rflect/concepts/proxy_concepts.hpp
         include/rflect/concepts/converter_closure.hpp
         include/rflect/concepts/enum_concepts.hpp
         # Containers
         include/rflect/containers/multi_array.hpp
         include/rflect/containers/multi_vector.hpp
         include/rflect/containers/dual_array.hpp
         include/rflect/containers/dual_vector.hpp
         include/rflect/containers/proxy.hpp
         include/rflect/containers/iterator.hpp
         include/rflect/containers/memory_layout.hpp
         include/rflect/containers/comparison.hpp
         # Converters
         include/rflect/converters/soa_to_zip.hpp
         include/rflect/converters/struct_to_soa.hpp
         include/rflect/converters/struct_to_tuple.hpp
         # Introspection
         include/rflect/introspection/enum.hpp
         include/rflect/introspection/struct.hpp
         include/rflect/introspection/for_each.hpp)

target_include_directories(rflect INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(rflect::rflect ALIAS rflect)
