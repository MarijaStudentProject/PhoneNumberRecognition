add_executable(normalizer_test test.cpp)
target_link_libraries(normalizer_test phone_normalizer)

enable_testing()
add_test(NAME normalizer_test COMMAND normalizer_test)

