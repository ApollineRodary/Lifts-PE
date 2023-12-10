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
)

add_executable(test tests/one_user_naive.cpp ${SOURCES})
add_executable(look tests/look_test.cpp ${SOURCES})
add_executable(test_random tests/random_tests.cpp ${SOURCES})
add_executable(one_src_multi_targ tests/evaluation_one_src_multi_targ.cpp ${SOURCES})