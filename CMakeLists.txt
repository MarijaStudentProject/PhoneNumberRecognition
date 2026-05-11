cmake_minimum_required(VERSION 3.10)
project(MyProject)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(dummy src/phone_number/address.cpp src/phone_number/phone_number.cpp src/phone_number/contact.cpp) 
target_include_directories(dummy PUBLIC "${CMAKE_SOURCE_DIR}/include" )
