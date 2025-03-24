function(add_sim_lib name)
    add_library(non-reflected-${name}-lib STATIC)

    target_sources(non-reflected-${name}-lib
            PRIVATE
            aos/grid.cpp
            aos/block.cpp
            aos/particle.cpp
            ../common/args/proargs.cpp
            ../common/file/fld.cpp
            ../simulator.cpp
            ../common/math/vector.cpp
            ../common/math/math.cpp
    )

    target_sources(non-reflected-${name}-lib
            PUBLIC
            FILE_SET HEADERS
            BASE_DIRS ..
            FILES
            aos/grid.hpp
            aos/block.hpp
            aos/particle.hpp
            ../simulator.hpp
#            ../common/math/concepts.hpp
#            ../common/math/functions.hpp
            ../common/math/math.hpp
#            ../common/math/matrix.hpp
#            ../common/math/operators.hpp
#            ../common/math/scalar.hpp
            ../common/math/utils.hpp
            ../common/math/vector.hpp
            ../common/utils/constants.hpp
            ../common/utils/error.hpp
            ../common/utils/primitive_types.hpp
    )

    target_include_directories(non-reflected-${name}-lib PUBLIC aos ../common)
    add_library(sim::non-reflected-${name}-lib ALIAS non-reflected-${name}-lib)
endfunction()

add_sim_lib(aos)
#add_sim_lib(soa)
