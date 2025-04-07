cmake_minimum_required(VERSION 3.30)
project(sim-tests CXX)


find_package(doctest REQUIRED)

macro(add_sim_test test_name test_src test_lib)
    list(APPEND TEST_FLAGS "--duration=true")

    set(JUNIT_DIR "${PROJECT_BINARY_DIR}/junit")
    file(MAKE_DIRECTORY ${JUNIT_DIR})
    set(JUNIT_FILE "${JUNIT_DIR}/${test_name}.xml")
    message("Setting junit file: ${JUNIT_FILE}")
    list(APPEND TEST_FLAGS "--reporters=junit")
    list(APPEND TEST_FLAGS "--out=${JUNIT_FILE}")

    set(test_exe_name "${test_name}-${test_lib}")
    add_executable(${test_exe_name} ${test_src})
    target_link_libraries(${test_exe_name} PRIVATE doctest::doctest ${test_lib}-lib)
    add_test(${test_name} ${test_exe_name} ${TEST_FLAGS})
endmacro(add_sim_test)

add_sim_test(functional_test ftest/fluid_simulator/test_correctness.cpp non-reflected-aos)
add_sim_test(functional_test ftest/fluid_simulator/test_correctness.cpp non-reflected-soa)
add_sim_test(dual_vector_test utest/runtime/test_dual_vector.cpp reflected-aos-soa)

add_library(unit_test_static STATIC
        utest/static/reflection_utilities.cpp
)

target_include_directories(unit_test_static PUBLIC ../src/reflected/reflection)

