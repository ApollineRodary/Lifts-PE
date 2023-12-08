cmake_minimum_required(VERSION 3.5)

project(lifts_pe)

include_directories(src)

set(SOURCES
    src/elevator_system.cpp
    src/elevator.cpp
    src/user.cpp
    src/simulation.cpp
    src/log.cpp
)

add_executable(test tests/one_user_naive.cpp ${SOURCES})
add_executable(scan tests/scan_test.cpp ${SOURCES})