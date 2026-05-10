cmake_minimum_required(VERSION 3.10)
project(MyProject)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(dummy src/dummy.cpp)

find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    file(GLOB_RECURSE ALL_CPP_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/tests/*.cpp
    )
    add_custom_target(tidy
        COMMAND ${CLANG_TIDY} -p ${CMAKE_BINARY_DIR} ${ALL_CPP_FILES}
        COMMENT "Running clang-tidy"
    )
endif()

find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/tests/*.cpp
        ${CMAKE_SOURCE_DIR}/include/*.hpp
    )
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
        COMMENT "Running clang-format"
    )
endif()

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})