cmake_minimum_required(VERSION 3.10)
project(PhoneNumberRecognition)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/phone_number/numbering_plan_repo.cpp
    src/phone_number/phone_normalizer.cpp
)

add_library(phone_normalizer STATIC ${SOURCES})
target_include_directories(phone_normalizer PUBLIC "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/libs")

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

add_subdirectory(tests)