cmake_minimum_required(VERSION 3.20)

project(
    SPSCRingBuffer
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    spsc
    INTERFACE
)

target_include_directories(
    spsc
    INTERFACE
    include
)

add_executable(
    hello
    examples/hello.cpp
)

target_link_libraries(
    hello
    PRIVATE
    spsc
)

add_executable(
    test_ring_buffer
    tests/test_ring_buffer.cpp
)

target_link_libraries(
    test_ring_buffer
    PRIVATE
    spsc
)

add_executable(
    test_spsc_queue
    tests/test_spsc_queue.cpp
)

target_link_libraries(
    test_spsc_queue
    PRIVATE
    spsc
)