#include <catch2/catch_test_macros.hpp>
#include <phone_number/address.hpp>

TEST_CASE("Address isEmpty", "[Address]") {
    SECTION("all fields empty returns true") { REQUIRE(Address{}.isEmpty()); }
    SECTION("each individual field set returns false") {
        REQUIRE_FALSE(Address("street").isEmpty());
        REQUIRE_FALSE(Address("", "10000").isEmpty());
        REQUIRE_FALSE(Address("", "", "city").isEmpty());
        REQUIRE_FALSE(Address("", "", "", "country").isEmpty());
    }
}

TEST_CASE("Address getFormatted", "[Address]") {
    SECTION("empty address returns empty string") { REQUIRE(Address{}.getFormatted().empty()); }
    SECTION("full address formats correctly") {
        Address a("21 Main St", "10000", "Novi Sad", "Serbia");
        REQUIRE(a.getFormatted() == "21 Main St, 10000 Novi Sad, Serbia");
    }
    SECTION("no street") {
        Address a("", "10000", "Novi Sad", "Serbia");
        REQUIRE(a.getFormatted() == "10000 Novi Sad, Serbia");
    }
    SECTION("no postal code") {
        Address a("21 Main St", "", "Novi Sad", "Serbia");
        REQUIRE(a.getFormatted() == "21 Main St, Novi Sad, Serbia");
    }
    SECTION("no city") {
        Address a("21 Main St", "10000", "", "Serbia");
        REQUIRE(a.getFormatted() == "21 Main St, 10000 , Serbia");
    }
    SECTION("no country") {
        Address a("21 Main St", "10000", "Novi Sad", "");
        REQUIRE(a.getFormatted() == "21 Main St, 10000 Novi Sad");
    }
    SECTION("only country") {
        Address a("", "", "", "Serbia");
        REQUIRE(a.getFormatted() == "Serbia");
    }
    SECTION("only street and country") {
        Address a("21 Main St", "", "", "Serbia");
        REQUIRE(a.getFormatted() == "21 Main St, Serbia");
    }
}

TEST_CASE("Address equality operator", "[Address]") {
    Address a("21 Main St", "10000", "Novi Sad", "Serbia");
    Address b("21 Main St", "10000", "Novi Sad", "Serbia");
    Address c("5 Other St", "20000", "Belgrade", "Serbia");

    SECTION("equal addresses") { REQUIRE(a == b); }
    SECTION("different addresses") { REQUIRE_FALSE(a == c); }
    SECTION("different only in street") {
        Address d("99 Other St", "10000", "Novi Sad", "Serbia");
        REQUIRE_FALSE(a == d);
    }
    SECTION("different only in postal code") {
        Address d("21 Main St", "99999", "Novi Sad", "Serbia");
        REQUIRE_FALSE(a == d);
    }
    SECTION("different only in city") {
        Address d("21 Main St", "10000", "Belgrade", "Serbia");
        REQUIRE_FALSE(a == d);
    }
    SECTION("different only in country") {
        Address d("21 Main St", "10000", "Novi Sad", "Germany");
        REQUIRE_FALSE(a == d);
    }
    SECTION("empty equals empty") { REQUIRE(Address{} == Address{}); }
    SECTION("empty does not equal non-empty") { REQUIRE_FALSE(Address{} == a); }
}
