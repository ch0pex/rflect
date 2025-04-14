function(add_reflected_lib name)
    add_library(reflected-${name}-lib STATIC)

    target_sources(reflected-${name}-lib
            PRIVATE
            aos-soa/grid.cpp
            aos-soa/block.cpp
    )

    target_sources(reflected-${name}-lib
            PUBLIC
            FILE_SET HEADERS
            BASE_DIRS ..
            FILES
            aos-soa/fld.hpp
            aos-soa/grid.hpp
            aos-soa/block.hpp
            aos-soa/particle.hpp
            ../simulator.hpp
            ../common/math/concepts.hpp
            ../common/math/functions.hpp
            ../common/math/math.hpp
            ../common/math/matrix.hpp
            ../common/math/operators.hpp
            ../common/math/scalar.hpp
            ../common/math/utils.hpp
            ../common/math/vector.hpp
            ../common/utils/constants.hpp
            ../common/utils/error.hpp
            ../common/utils/primitive_types.hpp
    )

    target_include_directories(reflected-${name}-lib PUBLIC aos-soa ../common)
    target_link_libraries(reflected-${name}-lib PUBLIC rflect::rflect)
    add_library(sim::reflected-${name}-lib ALIAS reflected-${name}-lib)
endfunction()

add_reflected_lib(aos)
add_reflected_lib(soa)
target_compile_definitions(reflected-soa-lib PUBLIC RFLECT_SOA=1)
