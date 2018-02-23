cmake_minimum_required(VERSION 2.6)
project(mcpe_server_test_1)

add_executable(mcpe_server_test_1 main.cpp)

install(TARGETS mcpe_server_test_1 RUNTIME DESTINATION bin)
