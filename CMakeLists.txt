cmake_minimum_required(VERSION 3.16)
project(PhoneNumberRecognition VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_QT_APP "Build Qt GUI application" ON)

include(FetchContent)

find_package(Catch2 3 QUIET)

if(NOT Catch2_FOUND)
    message(STATUS "Catch2 not found locally — fetching via FetchContent...")
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.7.1
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(Catch2)
else()
    message(STATUS "Catch2 found locally: ${Catch2_DIR}")
endif()

find_library(VCARD_LIB vCard REQUIRED)


set(NUMBER_SOURCES
    src/phone_number/numbering_plan_repo.cpp
    src/phone_number/phone_normalizer.cpp
    src/phone_number/address.cpp
    src/phone_number/phone_number.cpp
    src/phone_number/contact.cpp
    src/phone_number/vcf_parser.cpp
    src/phone_number/phone_matcher.cpp
)

add_library(phone_normalizer STATIC ${NUMBER_SOURCES})
target_include_directories(phone_normalizer PUBLIC "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/libs")
target_link_libraries(phone_normalizer PUBLIC ${VCARD_LIB})

add_subdirectory(tests)

set(RESOURCE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/resources)
set(RESOURCE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/resources)

add_custom_target(copy_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCE_SOURCE_DIR}
        ${RESOURCE_OUTPUT_DIR}
    COMMENT "Copying resources to build directory"
)

include(ClangTools.cmake)


if(BUILD_QT_APP)

    find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Multimedia Network)

    qt_add_executable(ContactsApp
        include/controller/contacts_controller.hpp
        include/model/contacts_model.hpp
        include/model/detailed_contact_model.hpp
        include/network/incoming_call_poller.hpp
        src/controller/contacts_controller.cpp
        src/model/contacts_model.cpp
        src/model/detailed_contact_model.cpp
        src/network/incoming_call_poller.cpp
        src/main.cpp
    )

    set_target_properties(ContactsApp PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
    )

    target_include_directories(ContactsApp PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/libs)

    add_dependencies(ContactsApp copy_resources)

    target_link_libraries(ContactsApp PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Multimedia
        Qt6::Network
        phone_normalizer
    )

    qt_add_qml_module(ContactsApp
        URI PhoneApp
        VERSION 1.0
        QML_FILES
            ui/Main.qml
            ui/ContactsView.qml
            ui/DialView.qml
            ui/ContactInfo.qml
            ui/DialingPopup.qml
            ui/IncomingCallPopup.qml
            ui/ActiveCallScreen.qml
        RESOURCES
            ui/fonts/fa-solid-900.ttf
            ui/sounds/dialpad-0.wav
            ui/sounds/dialpad-1.wav
            ui/sounds/dialpad-2.wav
            ui/sounds/dialpad-3.wav
            ui/sounds/dialpad-4.wav
            ui/sounds/dialpad-5.wav
            ui/sounds/dialpad-6.wav
            ui/sounds/dialpad-7.wav
            ui/sounds/dialpad-8.wav
            ui/sounds/dialpad-9.wav
            ui/sounds/click.wav
            ui/sounds/samsung-ringtone.wav
            ui/sounds/dialing-sound.wav
            ui/sounds/keyboard-sound.wav
    )
    
endif()