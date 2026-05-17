#include <catch2/catch_test_macros.hpp>
#include <phone_number/contact.hpp>
#include <phone_number/phone_number.hpp>
#include <phone_number/address.hpp>

TEST_CASE("Contact getFullName", "[Contact]") {
    SECTION("both name and surname") {
        Contact c("Marko", "Petrovic", {});
        REQUIRE(c.getFullName() == "Petrovic, Marko");
    }
    SECTION("only name") {
        Contact c("Marko", "", {});
        REQUIRE(c.getFullName() == "Marko");
    }
    SECTION("only surname") {
        Contact c("", "Petrovic", {});
        REQUIRE(c.getFullName() == "Petrovic");
    }
    SECTION("both empty") {
        Contact c("", "", {});
        REQUIRE(c.getFullName().empty());
    }
}

TEST_CASE("Contact hasPhoneNumbers", "[Contact]") {
    SECTION("no numbers returns false") {
        Contact c("Marko", "Petrovic", {});
        REQUIRE_FALSE(c.hasPhoneNumbers());
    }
    SECTION("with one number returns true") {
        PhoneNumber p("+38166123456", 381, "66123456", "+38166123456");
        Contact c("Marko", "Petrovic", {p});
        REQUIRE(c.hasPhoneNumbers());
    }
    SECTION("with multiple numbers returns true") {
        PhoneNumber p1("+38166111111", 381, "66111111", "+38166111111");
        PhoneNumber p2("+38166222222", 381, "66222222", "+38166222222");
        Contact c("Marko", "Petrovic", {p1, p2});
        REQUIRE(c.hasPhoneNumbers());
    }
}

TEST_CASE("Contact getPrimaryPhoneNumber", "[Contact]") {
    SECTION("empty list returns default PhoneNumber") {
        Contact c("Marko", "Petrovic", {});
        REQUIRE(c.getPrimaryPhoneNumber().getRawValue().empty());
    }
    SECTION("returns first number when one exists") {
        PhoneNumber p("+38166111111", 381, "66111111", "+38166111111");
        Contact c("Marko", "Petrovic", {p});
        REQUIRE(c.getPrimaryPhoneNumber().getRawValue() == "+38166111111");
    }
    SECTION("returns first number when multiple exist") {
        PhoneNumber p1("+38166111111", 381, "66111111", "+38166111111");
        PhoneNumber p2("+38166222222", 381, "66222222", "+38166222222");
        Contact c("Marko", "Petrovic", {p1, p2});
        REQUIRE(c.getPrimaryPhoneNumber().getRawValue() == "+38166111111");
    }
}

TEST_CASE("Contact addPhoneNumber", "[Contact]") {
    Contact c("Marko", "Petrovic", {});

    SECTION("adds new number") {
        PhoneNumber p("+38166111111", 381, "66111111", "+38166111111");
        c.addPhoneNumber(p);
        REQUIRE(c.getPhoneNumbers().size() == 1);
    }
    SECTION("adds two different numbers") {
        PhoneNumber p1("+38166111111", 381, "66111111", "+38166111111");
        PhoneNumber p2("+38166222222", 381, "66222222", "+38166222222");
        c.addPhoneNumber(p1);
        c.addPhoneNumber(p2);
        REQUIRE(c.getPhoneNumbers().size() == 2);
    }
    SECTION("does not add duplicate normalized value") {
        PhoneNumber p("+38166111111", 381, "66111111", "+38166111111");
        c.addPhoneNumber(p);
        c.addPhoneNumber(p);
        REQUIRE(c.getPhoneNumbers().size() == 1);
    }
    SECTION("raw differs but normalized same — treated as duplicate") {
        PhoneNumber local("066111111",    381, "66111111", "+38166111111");
        PhoneNumber intl("+38166111111",  381, "66111111", "+38166111111");
        c.addPhoneNumber(local);
        c.addPhoneNumber(intl);
        REQUIRE(c.getPhoneNumbers().size() == 1);
    }
    SECTION("adding to contact with existing numbers from constructor") {
        PhoneNumber p0("+38166000000", 381, "66000000", "+38166000000");
        PhoneNumber p1("+38166111111", 381, "66111111", "+38166111111");
        Contact c2("Marko", "Petrovic", {p0});
        c2.addPhoneNumber(p1);
        REQUIRE(c2.getPhoneNumbers().size() == 2);
    }
}

TEST_CASE("Contact hasAddress", "[Contact]") {
    SECTION("no address returns false") {
        Contact c("Marko", "Petrovic", {});
        REQUIRE_FALSE(c.hasAddress());
    }
    SECTION("empty address returns false") {
        Contact c("Marko", "Petrovic", {}, "", Address{});
        REQUIRE_FALSE(c.hasAddress());
    }
    SECTION("real address returns true") {
        Address a("21 Main St", "10000", "Novi Sad", "Serbia");
        Contact c("Marko", "Petrovic", {}, "", a);
        REQUIRE(c.hasAddress());
    }
}

TEST_CASE("Contact getAddress returns correct address", "[Contact]") {
    Address a("21 Main St", "10000", "Novi Sad", "Serbia");
    Contact c("Marko", "Petrovic", {}, "", a);

    REQUIRE(c.getAddress().getStreet()     == "21 Main St");
    REQUIRE(c.getAddress().getPostalCode() == "10000");
    REQUIRE(c.getAddress().getCity()       == "Novi Sad");
    REQUIRE(c.getAddress().getCountry()    == "Serbia");
}

TEST_CASE("Contact with empty name and surname", "[Contact]") {
    Contact c("", "", {});
    REQUIRE(c.getName().empty());
    REQUIRE(c.getSurname().empty());
    REQUIRE(c.getFullName().empty());
}
