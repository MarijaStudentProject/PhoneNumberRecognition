cmake_minimum_required(VERSION 3.10)
project(PhoneNumberRecognition)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)


find_package(Catch2 3 QUIET)

if(NOT Catch2_FOUND)
    message(STATUS "Catch2 not found locally — fetching via FetchContent...")
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.7.1
    )
    FetchContent_MakeAvailable(Catch2)
else()
    message(STATUS "Catch2 found locally: ${Catch2_DIR}")
endif()


set(SOURCES
    src/phone_number/numbering_plan_repo.cpp
    src/phone_number/phone_normalizer.cpp
    src/phone_number/address.cpp 
    src/phone_number/phone_number.cpp 
    src/phone_number/contact.cpp
    src/phone_number/vcf_parser.cpp
    src/phone_number/phone_matcher.cpp
)



find_library(VCARD_LIB vCard REQUIRED)

file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/include/*.hpp")
add_library(phone_normalizer STATIC ${SOURCES} ${HEADERS})
target_include_directories(phone_normalizer PUBLIC "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/libs")
target_link_libraries(phone_normalizer PUBLIC ${VCARD_LIB})

find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    file(GLOB_RECURSE ALL_CPP_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
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
        ${CMAKE_SOURCE_DIR}/libs/*.h
    )
    add_custom_target(format-fix
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
        COMMENT "Running clang-format"
    )
endif()

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})
set(RESOURCE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/resources)
set(RESOURCE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/resources)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
add_custom_target(copy_resources
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCE_SOURCE_DIR}
        ${RESOURCE_OUTPUT_DIR}
    COMMENT "Copying resources to build directory"
)

add_subdirectory(tests)


find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

qt_standard_project_setup()

qt_add_executable(ContactsApp src/controller/contacts_controller.cpp src/model/contacts_model.cpp
 include/controller/contacts_controller.hpp include/model/contacts_model.hpp src/model/detailed_contact_model.cpp include/model/detailed_contact_model.hpp
    src/main.cpp
)


target_link_libraries(ContactsApp PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick phone_normalizer)


find_package(Qt6 REQUIRED COMPONENTS Gui Quick QuickControls2)

set(CMAKE_AUTORCC ON)

set_target_properties(ContactsApp PROPERTIES
    AUTOMOC ON
)

qt_add_qml_module(ContactsApp
    URI PhoneApp
    VERSION 1.0
    QML_FILES
        ui/Main.qml
        ui/ContactsView.qml
        ui/DialView.qml
        QML_FILES  ui/ContactInfo.qml
        QML_FILES ui/DialingPopup.qml
        QML_FILES ui/IncomingCallPopup.qml
)

target_link_libraries(ContactsApp PRIVATE
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    phone_normalizer
)