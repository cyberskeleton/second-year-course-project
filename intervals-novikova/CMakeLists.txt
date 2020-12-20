cmake_minimum_required(VERSION 3.17)
project(intervals)

set(CMAKE_CXX_STANDARD 14)

add_executable(second-year-course-project intervals_c/tests/tests_c.c intervals_c/headers/interval.h intervals_c/headers/set_interval.h intervals_c/source/set_interval.c intervals_c/headers/tests_c.h main.cpp intervals_cpp/source/IntervalWrapper.cpp intervals_cpp/headers/IntervalWrapper.h intervals_cpp/source/SetIntervalWrapper.cpp intervals_cpp/headers/SetIntervalWrapper.h intervals_cpp/source/Service.cpp intervals_cpp/headers/Service.h intervals_cpp/tests/cross_tests.cpp intervals_cpp/headers/cross_tests.h intervals_cpp/source/Inequality.cpp intervals_cpp/headers/Inequality.h intervals_cpp/tests/ServiceTests.cpp intervals_cpp/headers/ServiceTests.h)