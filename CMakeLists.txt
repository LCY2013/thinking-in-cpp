cmake_minimum_required(VERSION 3.26)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "Project ID: ${ProjectId}")
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

#C
set(CMAKE_C_STANDARD 23)

#C++
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -o thinking-in-cpp -Wall -Wextra -pedantic -std=c++${CMAKE_CXX_STANDARD}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

## 手动模式
#set(BOOST_ROOT "/usr/local/include/boost")
##添加头文件搜索路径
#include_directories(/usr/local/include)
##添加库文件搜索路径
#link_directories(/usr/local/lib)
##在这里根据名字boost_thread去寻找libboost_thread.a文件
#target_link_libraries(${ProjectId} boost_thread boost_system)

#自动模式
message(STATUS "start running cmake...")

find_package(Boost COMPONENTS system filesystem REQUIRED)

#./bootstrap.sh
#sudo ./b2 install
#头文件 : /usr/local/include/boost
#库文件 : /usr/local/lib
if(Boost_FOUND)
    set(BOOST_ROOT "/usr/local/include/boost")
    set(Boost_LIBRARY_DIR "/usr/local/lib")
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIR}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIR})
endif()

set(SOURCE_FILES main.cpp)
if(Boost_FOUND)
#    target_link_libraries(${ProjectId} ${Boost_LIBRARIES})
endif()

#aux_source_directory(. DIR_SRCS)
#add_executable(${ProjectId} ${DIR_SRCS})
