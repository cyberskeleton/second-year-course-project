cmake_minimum_required(VERSION 3.17)
project(intervals)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled intervals_c/tests_c.c intervals_c/interval.h intervals_c/set_interval.h intervals_c/set_interval.c intervals_c/tests_c.h main.cpp intervals_cpp/IntervalWrapper.cpp intervals_cpp/IntervalWrapper.h intervals_cpp/SetIntervalWrapper.cpp intervals_cpp/SetIntervalWrapper.h intervals_cpp/service.cpp intervals_cpp/service.h intervals_cpp/cross_tests.cpp intervals_cpp/cross_tests.h)