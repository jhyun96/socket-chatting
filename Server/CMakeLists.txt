cmake_minimum_required(VERSION 3.26)
project(Socket_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
add_executable(${PROJECT_NAME}
        main.cpp
        socket_server.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ./include)