cmake_minimum_required(VERSION 3.10)
project(PhoneNumberRecognition)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/phone_number/numbering_plan_repo.cpp
    src/phone_number/phone_normalizer.cpp
    src/phone_number/address.cpp 
    src/phone_number/phone_number.cpp 
    src/phone_number/contact.cpp
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
    add_custom_target(tidy-fix
        COMMAND ${CLANG_TIDY} -p ${CMAKE_BINARY_DIR} --fix ${ALL_CPP_FILES}
        COMMENT "Running clang-tidy with --fix"
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
set(RESOURCE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/resources)
set(RESOURCE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/resources)

add_custom_target(copy_resources
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCE_SOURCE_DIR}
        ${RESOURCE_OUTPUT_DIR}
    COMMENT "Copying resources to build directory"
)

add_subdirectory(tests)
