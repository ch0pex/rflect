find_package(doctest REQUIRED)


macro(add_rflect_test test_name test_src)
    list(APPEND TEST_FLAGS "--duration=true")

    set(JUNIT_DIR "${PROJECT_BINARY_DIR}/junit")
    file(MAKE_DIRECTORY ${JUNIT_DIR})
    set(JUNIT_FILE "${JUNIT_DIR}/${test_name}.xml")
    message("Setting junit file: ${JUNIT_FILE}")
    list(APPEND TEST_FLAGS "--reporters=junit")
    list(APPEND TEST_FLAGS "--out=${JUNIT_FILE}")

    set(test_exe_name "${test_name}")
    add_executable(${test_exe_name} ${test_src})
    target_link_libraries(${test_exe_name} PRIVATE doctest::doctest rflect::rflect)
    add_test(${test_name} ${test_exe_name} ${TEST_FLAGS})
endmacro(add_rflect_test)


add_rflect_test(dual_vector_test test_dual_vector.cpp)
