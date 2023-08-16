cmake_minimum_required(VERSION 3.27...3.28)

project(Safe_Que LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(safe_que main.cpp)
target_sources(safe_que PUBLIC que.h)

add_test(NAME Safe_Que COMMAND safe_que)