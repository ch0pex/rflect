cmake_minimum_required(VERSION 3.28)
project(sim-tests)

include(FetchContent)

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)


macro(add_sim_utest test_name test_src)
    list(APPEND TEST_FLAGS "--duration=true")

    set(JUNIT_DIR "${PROJECT_BINARY_DIR}/junit")
    file(MAKE_DIRECTORY ${JUNIT_DIR})
    set(JUNIT_FILE "${JUNIT_DIR}/${test_name}.xml")
    message("Setting junit file: ${JUNIT_FILE}")
    list(APPEND TEST_FLAGS "--reporters=junit")
    list(APPEND TEST_FLAGS "--out=${JUNIT_FILE}")

    set(test_exe_name "sim_${test_name}")
    add_executable(${test_exe_name} ${test_src})
    target_link_libraries(${test_exe_name} PRIVATE doctest::doctest)
    add_test(${test_name} ${test_exe_name} ${TEST_FLAGS})
endmacro(add_sim_utest)

macro(add_sim_ftest test_name test_src)
    set(test_exe_name "sim_${test_name}")
    include(../Source/cmake/editor.cmake)
    include(../Source/cmake/engine.cmake)
    set(BASE "${CMAKE_CURRENT_SOURCE_DIR}/../Source")
    list(TRANSFORM EDITOR_SOURCES PREPEND "${BASE}/")
    list(TRANSFORM ENGINE_SOURCES PREPEND "${BASE}/")
    add_executable(${test_exe_name} ${test_src} ) # TODO
endmacro(add_sim_ftest)

add_sim_utest(utest_math utest/math.cpp)