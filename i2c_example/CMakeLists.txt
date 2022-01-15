cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(example_i2c C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(example example.c)

pico_enable_stdio_uart(example 1)
pico_enable_stdio_usb(example 1)

pico_add_extra_outputs(example)

target_compile_options(example PRIVATE -Wall)

target_link_libraries(example pico_stdlib hardware_i2c pico_multicore)
