
aux_source_directory(common/args COMMON_ARGS)
aux_source_directory(common/file COMMON_FILE)
aux_source_directory(common/math COMMON_MATH)
aux_source_directory(common/utils COMMON_UTILS)

set(COMMON_SOURCES ${COMMON_ARGS} ${COMMON_FILE} ${COMMON_MATH} ${COMMON_UTILS})
set(SIM_SOURCES main.cpp simulator.cpp simulator.hpp ${COMMON_SOURCES})

include(reflected/reflected.cmake)
include(non-reflected/non-reflected.cmake)

# --- Executables ---
add_executable(Reflected-aos ${SIM_SOURCES} ${REFLECTED_AOS} ${REFLECTED_AOS})
target_include_directories(Reflected-aos PUBLIC reflected/reflection reflected/aos-soa)
target_compile_definitions(Reflected-aos PUBLIC AOS)

add_executable(Reflected-soa ${SIM_SOURCES} ${REFLECTED_AOS} ${REFLECTED_SOA})
target_include_directories(Reflected-soa PUBLIC reflected/reflection reflected/aos-soa)
target_compile_definitions(Reflected-aos PUBLIC SOA)

add_executable(Reflected-mixed ${SIM_SOURCES} ${REFLECTED_AOS} ${REFLECTED_MIXED})
target_include_directories(Reflected-mixed PUBLIC reflected/reflection reflected/mixed)

add_executable(Non-reflected-aos ${SIM_SOURCES}  ${NON_REFLECTED_AOS} ${NON_REFLECTED_AOS})
target_include_directories(Non-reflected-aos PUBLIC non-reflected/aos)

add_executable(Non-reflected-soa ${SIM_SOURCES} ${NON_REFLECTED_AOS} ${NON_REFLECTED_SOA})
target_include_directories(Non-reflected-soa PUBLIC non-reflected/soa)
