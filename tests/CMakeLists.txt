
enable_testing()

add_executable(test test.cpp)
target_link_libraries(test phone_normalizer)
add_test(NAME test COMMAND test)

