function(add_reflected_lib name)
    add_library(reflected-${name}-lib STATIC)

    target_sources(reflected-${name}-lib
            PRIVATE
            ${name}/grid.cpp
            ${name}/block.cpp
    )

    target_sources(reflected-${name}-lib
            PUBLIC
            FILE_SET HEADERS
            BASE_DIRS ..
            FILES
            ${name}/grid.hpp
            ${name}/block.hpp
            ${name}/particle.hpp
            ../simulator.hpp
            reflection/concepts.hpp
            reflection/dual_vector.hpp
            reflection/proxy_class.hpp
            reflection/utils.hpp
            reflection/dual_vector_detail.hpp
            reflection/memory_layout.hpp
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

    target_include_directories(reflected-${name}-lib PUBLIC ${name} ../common)
    add_library(sim::reflected-${name}-lib ALIAS reflected-${name}-lib)
endfunction()

add_reflected_lib(aos-soa)
#add_reflected_lib(soa)
