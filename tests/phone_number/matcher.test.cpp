#include <catch2/catch_test_macros.hpp>

TEST_CASE("match by prefix", "[match]") {
    
    SECTION("normal prefix match") {
        REQUIRE(2+3 == 5);
    }

    SECTION("empty prefix match check") {
        REQUIRE(2+2==4);
    }

    SECTION("on empty prefix throws") {
        REQUIRE_THROWS_AS(2+3, std::invalid_argument);
    }

}

TEST_CASE("find by name", "[match]") {

    SECTION("normal test") {
        REQUIRE(2==2);
    }

    
}