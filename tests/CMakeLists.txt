add_executable(normalizer_test test.cpp)
target_link_libraries(normalizer_test phone_normalizer)


set(TEST_SOURCES
    phone_number/numbering_plan_repo.test.cpp
    phone_number/normalizer.test.cpp
    phone_number/vcf_parser.test.cpp
    phone_number/matcher.test.cpp
)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE phone_normalizer Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)
