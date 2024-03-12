cmake_minimum_required(VERSION 3.22)

project(DataHandle)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/utf-8)
endif()

add_executable(
        STLSample
        STLSample/src/main.cpp
)

add_executable(
        RangesSample
        RangesSample/src/main.cpp
)

add_executable(
        RangesIteratorSample
        RangesIteratorSample/src/main.cpp
)

add_executable(
        RangesSizeSample
        RangesSizeSample/src/main.cpp
)
#[[
add_executable(
        RangesDataSample
        RangesDataSample/src/main.cpp
)

add_executable(
        RangesDanglingFailed
        RangesDanglingFailed/src/main.cpp
)

add_executable(
        RangesDanglingCheck
        RangesDanglingCheck/src/main.cpp
)

add_executable(
        ViewBasic
        ViewBasic/src/main.cpp
)

add_executable(
        ArrayView
        ArrayView/src/main.cpp
)

add_executable(
        FactorySample
        FactorySample/src/main.cpp
)

add_executable(
        AdaptorSample
        AdaptorSample/src/main.cpp
)

add_executable(
        STLRandom
        STLRandom/src/main.cpp
)

add_executable(
        PipelineSample
        PipelineSample/src/main.cpp
)]]
