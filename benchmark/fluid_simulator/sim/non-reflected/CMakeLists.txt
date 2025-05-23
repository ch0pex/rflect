function(add_sim_lib name)
    add_library(non-reflected-${name}-lib STATIC)

    target_sources(non-reflected-${name}-lib
            PRIVATE
            ${name}/grid.cpp
            ${name}/block.cpp
    )

    target_sources(non-reflected-${name}-lib
            PUBLIC
            FILE_SET HEADERS
            BASE_DIRS ..
            FILES
            ${name}/grid.hpp
            ${name}/block.hpp
            ${name}/particle.hpp
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

    target_include_directories(non-reflected-${name}-lib PUBLIC ${name} ../common)
    add_library(sim::non-reflected-${name}-lib ALIAS non-reflected-${name}-lib)
endfunction()

add_sim_lib(aos)
add_sim_lib(soa)
