cmake_minimum_required(VERSION 2.6)
project(mcpe_server_test_1)

set(SOURCE_FILES
src/main.cpp
src/udpserver.cpp)

find_package(Threads REQUIRED)

add_executable(mcpe_server_test_1 ${SOURCE_FILES})

target_link_libraries(mcpe_server_test_1 ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS mcpe_server_test_1 RUNTIME DESTINATION bin)
