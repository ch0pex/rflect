add_subdirectory(sim/non-reflected)
add_subdirectory(sim/reflected)

macro(add_sim_executable name)
    add_executable(${name} main.cpp)
    target_link_libraries(${name} PRIVATE sim::${name}-lib)
endmacro()

add_sim_executable(non-reflected-aos)
add_sim_executable(non-reflected-soa)
add_sim_executable(reflected-aos-soa)
