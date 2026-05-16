#include <catch2/catch_test_macros.hpp>
#include <phone_number/phone_number.hpp>

TEST_CASE("PhoneNumber default constructor", "[PhoneNumber]") {
    PhoneNumber p;
    REQUIRE(p.getRawValue().empty());
    REQUIRE(p.getNormalizedValue().empty());
    REQUIRE(p.getCountryCode() == 0);
    REQUIRE(p.getNationalNumber().empty());
}

TEST_CASE("PhoneNumber constructor stores all fields correctly", "[PhoneNumber]") {
    PhoneNumber p("+38166123456", 381, "66123456", "+38166123456");
    REQUIRE(p.getRawValue() == "+38166123456");
    REQUIRE(p.getNormalizedValue() == "+38166123456");
    REQUIRE(p.getCountryCode() == 381);
    REQUIRE(p.getNationalNumber() == "66123456");
}

TEST_CASE("PhoneNumber raw and normalized can differ", "[PhoneNumber]") {
    PhoneNumber p("066123456", 381, "66123456", "+38166123456");
    REQUIRE(p.getRawValue() != p.getNormalizedValue());
}

TEST_CASE("PhoneNumber with empty normalized value", "[PhoneNumber]") {
    PhoneNumber p("abc123", 0, "", "");
    REQUIRE(p.getRawValue() == "abc123");
    REQUIRE(p.getNormalizedValue().empty());
    REQUIRE(p.getCountryCode() == 0);
    REQUIRE(p.getNationalNumber().empty());
}

TEST_CASE("PhoneNumber with different country codes", "[PhoneNumber]") {
    SECTION("Serbian number") {
        PhoneNumber p("+38166123456", 381, "66123456", "+38166123456");
        REQUIRE(p.getCountryCode() == 381);
    }
    SECTION("US number") {
        PhoneNumber p("+12125551234", 1, "2125551234", "+12125551234");
        REQUIRE(p.getCountryCode() == 1);
    }
    SECTION("German number") {
        PhoneNumber p("+4930123456", 49, "30123456", "+4930123456");
        REQUIRE(p.getCountryCode() == 49);
    }
}

TEST_CASE("PhoneNumber with zero country code", "[PhoneNumber]") {
    PhoneNumber p("unknownnumber", 0, "", "unknownnumber");
    REQUIRE(p.getCountryCode() == 0);
    REQUIRE(p.getRawValue() == "unknownnumber");
    REQUIRE(p.getNormalizedValue() == "unknownnumber");
}

TEST_CASE("PhoneNumber all empty strings", "[PhoneNumber]") {
    PhoneNumber p("", 0, "", "");
    REQUIRE(p.getRawValue().empty());
    REQUIRE(p.getNormalizedValue().empty());
    REQUIRE(p.getNationalNumber().empty());
    REQUIRE(p.getCountryCode() == 0);
}
