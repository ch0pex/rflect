add_subdirectory(non-reflected)
add_subdirectory(reflected)

function(add_sim_executable name)
    add_executable(${name})

    target_sources(${name}
            PRIVATE
            main.cpp
            simulator.cpp
    )
    target_sources(${name}
            PUBLIC
            FILE_SET HEADERS
            BASE_DIRS .
            FILES
            simulator.hpp
    )
    target_link_libraries(${name} PRIVATE sim::${name}-lib)
endfunction()

add_sim_executable(non-reflected-aos)