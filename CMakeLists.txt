cmake_minimum_required(VERSION 3.12)

# Symlink this file into the project directory
include(pico_sdk_import.cmake)

project(test-pico-keypad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

include(../pimoroni-pico/libraries/pico_rgb_keypad/pico_rgb_keypad.cmake)

add_executable(test-pico-keypad
    main.cpp
)

target_link_libraries(test-pico-keypad
    pico_rgb_keypad)

pico_add_extra_outputs(test-pico-keypad)
