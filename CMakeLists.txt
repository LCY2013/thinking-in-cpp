#https://cmake.org/
#cmake_minimum_required 指令指定支持该 CMakeLists.txt 文件的 cmake 最低版本号
cmake_minimum_required(VERSION 3.26)
set (TARGET thinking-in-cpp)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "Project ID: ${ProjectId}")
string(REPLACE " " "_" ProjectId ${ProjectId})
#project 指令指定该项目的名称，注意项目名称不是最终生成的二进制程序名，一个项目下面可以生成多个二进制程序名
project(${ProjectId})

#set 定义和设置各种变量，set 括号后第一个名称是定义的变量名称，其后是变量的值，
#如定义了 CMAKE_CXX_FLAGS、net_srcs、mysqlapi_src、mysqlmgr_srcs 、json_srcs、zlib_srcs、utils_srcs、chatserver_srcs、fileserver_srcs、imgserver_srcs 一共 10 个变量，
#之后引用这些变量可以使用 ${变量名} 来引用这些变量，这些变量可以是内置变量，
#如 CMAKE_CXX_FLAGS 指定 g++ 编译选项，EXECUTABLE_OUTPUT_PATH 指定输出的二进制文件路径，也可以是自定义变量，
#如 chatserver_srcs、fileserver_srcs 等

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

#set(SOURCE_FILES main.cpp)
if(Boost_FOUND)
#    target_link_libraries(${ProjectId} ${Boost_LIBRARIES})
endif()

#cmake 使用 aux_source_directory 指令指定源码目录，使用 include_directories 指令指定包含目录，使用link_directories 指定 lib 目录；
#cmake 使用 指令指定生成的动态或静态库的名称，其格式如下：
#  add_library(libname [SHARED|STATIC|MODULE] [EXCLUDE_FROM_ALL] source1 source2 … sourceN)
#  例如：add_library(hello hello1.cpp hello2.cpp)

#不需要写全 libhello.so 或 libhello.a，只需要填写 hello 即可，cmake 系统会自动为你生成 libhello.X。类型有三种：
#SHARED，动态库(扩展名为 .so)
#STATIC，静态库(扩展名为 .a)
#MODULE，在使用 dyld 的系统有效，如果不支持dyld，则被当作 SHARED 对待。

#EXCLUDE_FROM_ALL 参数的意思是这个库不会被默认构建，除非有其他的组件依赖或者手工构建。
#下面命令会生成一个叫 libkafkawrapper.so 的文件，且 libkafkawrapper.so 的生成依赖 librdkafka.so、librdkafka++.so、libcrypto.so、libssl.so 四个库。
#add_library(kafkawrapper SHARED ${kafka_wrapper_srcs})
#TARGET_LINK_LIBRARIES(kafkawrapper rdkafka rdkafka++ crypto ssl)

#TARGET_LINK_LIBRARIES 指定生成的二进制文件以来的其他库，上文已有介绍；
#编写完 CMakeLists.txt 文件后，进入 CMakeLists.txt 文件依次执行如下命令即可生成最终的二进制文件：

# 利用cmake生成makefile
#cmake .
## 执行make命令，利用gcc/g++编译生成最终的二进制文件
#make

#CMakeLists.txt 也支持递归编译，父目录的 CMakeLists 先执行再接着执行子目录的 CMakeLists.txt 文件。


#aux_source_directory(. DIR_SRCS)
#add_executable(${ProjectId} ${DIR_SRCS})

#function (prepare_for_module TGT)
#    target_compile_options(${TGT} PUBLIC -fmodules-ts)
#    set_property(TARGET ${TGT} PROPERTY CXX_STANDARD 20)
#    set_property(TARGET ${TGT} PROPERTY CXX_EXTENSIONS OFF)
#    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#        add_dependencies(${TGT} std_modules)
#    endif()
#endfunction()
#
#set (MODULE_EXT cpp)
#if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#    set (MODULE_EXT ixx)
#    add_compile_options(/utf-8)
#endif()

ADD_SUBDIRECTORY(cpp20/module/modulev1)
ADD_SUBDIRECTORY(cpp20/module/ModuleGettingStarted)
ADD_SUBDIRECTORY(cpp20/module/MultiModules)
ADD_SUBDIRECTORY(cpp20/module/ModuleInclude)
ADD_SUBDIRECTORY(cpp20/module/GlobalModuleFragment)
ADD_SUBDIRECTORY(cpp20/module/PrivateModuleFragment)
ADD_SUBDIRECTORY(cpp20/module/ModulePartition)
ADD_SUBDIRECTORY(cpp20/module/ImageServer)
ADD_SUBDIRECTORY(cpp20/genericity/BasicTemplate)
ADD_SUBDIRECTORY(cpp20/genericity/Specializations)
ADD_SUBDIRECTORY(cpp20/genericity/ParamterPack)
ADD_SUBDIRECTORY(cpp20/genericity/TypeTraits)
ADD_SUBDIRECTORY(cpp20/concepts/BasicConcepts)
ADD_SUBDIRECTORY(cpp20/concepts/Conjunctions)
ADD_SUBDIRECTORY(cpp20/concepts/Disjunctions)
ADD_SUBDIRECTORY(cpp20/concepts/AtomicConstraints)
ADD_SUBDIRECTORY(cpp20/concepts/RequiresExpressions)
ADD_SUBDIRECTORY(cpp20/concepts/RequiresClauses)
ADD_SUBDIRECTORY(cpp20/concepts/NumCpp)
ADD_SUBDIRECTORY(cpp20/Coroutines/BasicThread)
ADD_SUBDIRECTORY(cpp20/Coroutines/BasicFileSystem)
ADD_SUBDIRECTORY(cpp20/Coroutines/FutureFileSystem)

#if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#    prepare_for_module(${TARGET})
#endif()
