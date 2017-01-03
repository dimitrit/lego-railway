cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME lego_railway)
project(${PROJECT_NAME})

set(ARDUINO_SKETCH_FOLDER ~/Documents/Arduino)
link_directories(${ARDUINO_SKETCH_FOLDER}/libraries)

link_directories(~/Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/)
link_directories(~/Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/)

set(Servo_RECURSE True)
set(PowerFunctions_RECURSE True)

set(ARDUINO_DEFAULT_BOARD nano328b)
set(ARDUINO_DEFAULT_PORT /dev/cu.wchusbserial1420)

set(${CMAKE_PROJECT_NAME}_SKETCH lego_railway.ino)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
target_link_libraries(${CMAKE_PROJECT_NAME} ${ARDUINO_DEFAULT_BOARD}_SignalController ${ARDUINO_DEFAULT_BOARD}_DispatchQueue ${ARDUINO_DEFAULT_BOARD}_Points ${ARDUINO_DEFAULT_BOARD}_Servo ${ARDUINO_DEFAULT_BOARD}_TrainController ${ARDUINO_DEFAULT_BOARD}_PowerFunctions)
