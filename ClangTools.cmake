find_program(CLANG_TIDY clang-tidy)

if(CLANG_TIDY)
    if (BUILD_QT_APP)
        file(GLOB_RECURSE ALL_CPP_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/include/*.hpp
    )    
    else()
        file(GLOB_RECURSE ALL_CPP_FILES
        ${CMAKE_SOURCE_DIR}/src/phone_number/*.cpp
        ${CMAKE_SOURCE_DIR}/include/phone_number/*.hpp
    )
    endif()
    

    
    add_custom_target(tidy-check
        COMMAND ${CLANG_TIDY}
            -p ${CMAKE_BINARY_DIR}
            ${ALL_CPP_FILES}
    )

    add_custom_target(tidy-fix
        COMMAND ${CLANG_TIDY}
            -p ${CMAKE_BINARY_DIR}
            --fix
            ${ALL_CPP_FILES}
    )
endif()

find_program(CLANG_FORMAT clang-format)

if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/tests/*.cpp
        ${CMAKE_SOURCE_DIR}/include/*.hpp
    )

    add_custom_target(format-fix
        COMMAND ${CLANG_FORMAT}
            -i
            ${ALL_SOURCE_FILES}
    )
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT}
            --dry-run
            --Werror
            ${ALL_SOURCE_FILES}
    )
endif()