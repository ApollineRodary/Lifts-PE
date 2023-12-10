cmake_minimum_required(VERSION 3.5)

project(lifts_pe)

include_directories(src)

set(SOURCES
    src/elevator_system.cpp
    src/elevator.cpp
    src/user.cpp
    src/simulation.cpp
    src/log.cpp
    src/generation/random_functions.cpp
    src/generation/user_generation.cpp
    src/user_presets/pe_group.cpp
)

add_executable(test tests/one_user_naive.cpp ${SOURCES})
add_executable(look tests/look_test.cpp ${SOURCES})
add_executable(scan tests/scan_test.cpp ${SOURCES})
add_executable(test_random tests/random_tests.cpp ${SOURCES})
add_executable(one_src_multi_targ tests/evaluation_one_src_multi_targ.cpp ${SOURCES})
add_executable(one_src_multi_targ_two tests/one_src_multi_targ_two_elevators.cpp ${SOURCES}) 
add_executable(realistic_look tests/realistic_look.cpp ${SOURCES})
add_executable(realistic_scan tests/realistic_scan.cpp ${SOURCES})
add_executable(realistic_look_two tests/realistic_look_two_elev.cpp ${SOURCES})