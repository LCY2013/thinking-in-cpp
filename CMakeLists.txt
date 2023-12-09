cmake_minimum_required(VERSION 3.26)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "Project ID: ${ProjectId}")
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

set(CMAKE_C_STANDARD 23)
#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -o thinking-in-cpp -Wall -Wextra -pedantic -std=c++17")

