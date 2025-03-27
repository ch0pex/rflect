cmake_minimum_required(VERSION 3.30)
project(sim-tests)


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

add_sim_test(functional_test ftest/main.cpp non-reflected-aos)
add_sim_test(functional_test ftest/main.cpp non-reflected-soa)

#add_sim_test(utest_block utest/test_block.cpp)
#add_sim_test(utest_grid utest/test_grid.cpp)
#add_sim_test(utest_proargs utest/test_proargs.cpp)
