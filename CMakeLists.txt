cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(LineFollowerBot C CXX ASM)
set(CMAKE_CXX_STANDARD 23)

pico_sdk_init()

add_executable(LineFollowerBot src/main.cpp
        src/io/PowerLED.cpp
        src/io/PowerLED.h
        src/io/Log.cpp
        src/io/Log.h
        src/io/motor/DCGearMotor.cpp
        src/io/motor/DCGearMotor.h
)

# Console over USB instead of the DEBUG Pins
# "sudo chmod 666 /dev/ttyACM0" If it fails
pico_enable_stdio_usb(LineFollowerBot 1)
pico_enable_stdio_uart(LineFollowerBot 0)

target_link_libraries(LineFollowerBot pico_stdlib hardware_gpio hardware_pwm)

pico_add_extra_outputs(LineFollowerBot)