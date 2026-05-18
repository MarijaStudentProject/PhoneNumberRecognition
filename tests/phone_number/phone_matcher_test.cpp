#include "phone_number/contact.hpp"
#include "phone_number/phone_matcher.hpp"
#include "phone_number/phone_number.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("PhoneMatcher isExactMatch", "[matcher]") {
    PhoneNumber p1("+38166123456", 381, "66123456", "+38166123456");
    PhoneNumber p2("+38166123456", 381, "66123456", "+38166123456");
    PhoneNumber p3("+38166000000", 381, "66000000", "+38166000000");
    PhoneNumber raw1("066123456", 0, "", "");

    SECTION("same normalized values") { REQUIRE(PhoneMatcher::isExactMatch(p1, p2)); }

    SECTION("different normalized values") { REQUIRE_FALSE(PhoneMatcher::isExactMatch(p1, p3)); }

    SECTION("first has no normalized value") { REQUIRE_FALSE(PhoneMatcher::isExactMatch(raw1, p1)); }

    SECTION("both have no normalized value") {
        PhoneNumber raw2("0038166000000", 0, "", "");
        REQUIRE_FALSE(PhoneMatcher::isExactMatch(raw1, raw2));
    }
}

TEST_CASE("PhoneMatcher findMatch", "[matcher]") {
    PhoneNumber num("+38166123456", 381, "66123456", "+38166123456");
    PhoneNumber matchPhone("+38166123456", 381, "66123456", "+38166123456");
    PhoneNumber differentPhone("+38166000000", 381, "66000000", "+38166000000");

    SECTION("empty contacts") {
        std::vector<Contact> contacts;
        REQUIRE_FALSE(PhoneMatcher::findMatch(num, contacts).has_value());
    }

    SECTION("exact match, first contact has no phones") {
        std::vector<Contact> contacts = {Contact("Ana", "Anic", {}), Contact("Marko", "Petrovic", {matchPhone})};
        auto result = PhoneMatcher::findMatch(num, contacts);
        REQUIRE(result.has_value());
        REQUIRE(result.value() == 1);
    }

    SECTION("no exact match, falls back to suffix match") {
        // last 11 out of 13 digits match
        PhoneNumber contactPhone("0038166123456", 381, "66123456", "+49000000000");
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {contactPhone})};
        auto result = PhoneMatcher::findMatch(num, contacts);
        REQUIRE(result.has_value());
        REQUIRE(result.value() == 0);
    }

    SECTION("no match at all") {
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {differentPhone})};
        REQUIRE_FALSE(PhoneMatcher::findMatch(num, contacts).has_value());
    }

    SECTION("raw number skips exact match and uses suffix search") {
        // rawNum has no normalized value so isExactMatch always returns false
        PhoneNumber rawNum("66123456", 0, "", "");
        // matches last 8 out of 10 digits
        PhoneNumber contactPhone("0066123456", 381, "66123456", "+38166123456");
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {contactPhone})};
        auto result = PhoneMatcher::findMatch(rawNum, contacts);
        REQUIRE(result.has_value());
        REQUIRE(result.value() == 0);
    }
}

TEST_CASE("PhoneMatcher matchBySuffix", "[matcher]") {
    SECTION("number equals phone raw value") {
        std::string number = "66123456"; // 8 chars
        PhoneNumber contactPhone("66123456", 381, "66123456", "+38166123456");
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {contactPhone})};
        auto result = PhoneMatcher::matchBySuffix(number, contacts, 5, 0.75F);
        REQUIRE(result.has_value());
        REQUIRE(result.value() == 0);
    }

    SECTION("only second contact matches") {
        std::string number = "66123456"; // 8 chars
        PhoneNumber noMatch("000000000", 0, "", "000000000");
        PhoneNumber match("0066123456", 381, "66123456", "+38166123456"); // 10 chars, 8/10=0.8≥0.75
        std::vector<Contact> contacts = {Contact("Ana", "Anic", {noMatch}), Contact("Marko", "Petrovic", {match})};
        auto result = PhoneMatcher::matchBySuffix(number, contacts, 5, 0.75F);
        REQUIRE(result.has_value());
        REQUIRE(result.value() == 1);
    }

    SECTION("number shorter than minLen, loop never executes") {
        std::string number = "1234"; // 4 chars, minLen=5
        PhoneNumber contactPhone("01234", 0, "", "01234");
        std::vector<Contact> contacts = {Contact("Ana", "Anic", {contactPhone})};
        REQUIRE_FALSE(PhoneMatcher::matchBySuffix(number, contacts, 5, 0.75F).has_value());
    }

    SECTION("threshold on phone side fails") {
        // 12345 len 5, phone.raw "000000012345" len 12, 5/12 less then treshold
        std::string number = "12345";
        PhoneNumber contactPhone("000000012345", 0, "", "000000012345");
        std::vector<Contact> contacts = {Contact("Ana", "Anic", {contactPhone})};
        REQUIRE_FALSE(PhoneMatcher::matchBySuffix(number, contacts, 5, 0.75F).has_value());
    }

    SECTION("empty contacts") {
        std::string number = "66123456";
        std::vector<Contact> contacts;
        REQUIRE_FALSE(PhoneMatcher::matchBySuffix(number, contacts, 5, 0.75F).has_value());
    }

    SECTION("contact without phone numbers") {
        std::string number = "66123456";
        std::vector<Contact> contacts = {Contact("Ana", "Anic", {})};
        REQUIRE_FALSE(PhoneMatcher::matchBySuffix(number, contacts, 5, 0.75F).has_value());
    }
}

TEST_CASE("PhoneMatcher searchByName", "[matcher]") {
    PhoneNumber dummy("+38166123456", 381, "66123456", "+38166123456");

    SECTION("exact full name match") {
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {dummy})};
        auto result = PhoneMatcher::searchByName("Petrovic, Marko", contacts, 2);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == 0);
    }

    SECTION("case insensitive match") {
        std::vector<Contact> contacts = {Contact("marko", "petrovic", {dummy})};
        auto result = PhoneMatcher::searchByName("MARKO", contacts, 2);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == 0);
    }

    SECTION("one typo within threshold") {
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {dummy})};
        auto result = PhoneMatcher::searchByName("Perovic", contacts, 2);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == 0);
    }

    SECTION("two typos at threshold boundary") {
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {dummy})};
        auto result = PhoneMatcher::searchByName("Markuu", contacts, 2);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == 0);
    }

    SECTION("above threshold, no match") {
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {dummy})};
        auto result = PhoneMatcher::searchByName("Xxxxxx", contacts, 2);
        REQUIRE(result.empty());
    }

    SECTION("multiple contacts match") {
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {dummy}), Contact("Marco", "Petrovic", {dummy})};
        auto result = PhoneMatcher::searchByName("Marko", contacts, 2);
        REQUIRE(result.size() == 2);
        REQUIRE(result[0] == 0);
        REQUIRE(result[1] == 1);
    }

    SECTION("empty contacts") {
        std::vector<Contact> contacts;
        auto result = PhoneMatcher::searchByName("Marko", contacts, 2);
        REQUIRE(result.empty());
    }

    SECTION("unicode name match within threshold") {
        std::vector<Contact> contacts = {Contact("Марко", "Петровић", {dummy})};
        auto result = PhoneMatcher::searchByName("Марк", contacts, 2);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == 0);
    }
}

TEST_CASE("PhoneMatcher searchByNumberPrefix", "[matcher]") {
    SECTION("prefix matches raw value") {
        // raw starts with 003
        PhoneNumber p("0038166123456", 381, "66123456", "+38166123456");
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {p})};
        auto result = PhoneMatcher::searchByNumberPrefix("003", contacts);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == 0);
    }

    SECTION("prefix matches normalized value only") {
        // raw does not start with +381, normalized does
        PhoneNumber p("0038166123456", 381, "66123456", "+38166123456");
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {p})};
        auto result = PhoneMatcher::searchByNumberPrefix("+381", contacts);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == 0);
    }

    SECTION("full number as prefix") {
        PhoneNumber p("+38166123456", 381, "66123456", "+38166123456");
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {p})};
        auto result = PhoneMatcher::searchByNumberPrefix("+38166123456", contacts);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == 0);
    }

    SECTION("no contact matches prefix") {
        PhoneNumber p1("+38166123456", 381, "66123456", "+38166123456");
        PhoneNumber p2("+49301234567", 49, "301234567", "+49301234567");
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {p1}), Contact("Hans", "Mueller", {p2})};
        auto result = PhoneMatcher::searchByNumberPrefix("", contacts);
        REQUIRE(result.size() == 2);
        REQUIRE(result[0] == 0);
        REQUIRE(result[1] == 1);
    }

    SECTION("prefix longer than number returns no match") {
        PhoneNumber p("+38166123456", 381, "66123456", "+38166123456");
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {p})};
        auto result = PhoneMatcher::searchByNumberPrefix("+381661234560000", contacts);
        REQUIRE(result.empty());
    }

    SECTION("multiple contacts match same prefix") {
        PhoneNumber p1("+38166123456", 381, "66123456", "+38166123456");
        PhoneNumber p2("+38160123456", 381, "60123456", "+38160123456");
        std::vector<Contact> contacts = {Contact("Marko", "Petrovic", {p1}), Contact("Ana", "Anic", {p2})};
        auto result = PhoneMatcher::searchByNumberPrefix("+381", contacts);
        REQUIRE(result.size() == 2);
        REQUIRE(result[0] == 0);
        REQUIRE(result[1] == 1);
    }

    SECTION("contact without phone numbers is skipped") {
        std::vector<Contact> contacts = {Contact("Ana", "Anic", {})};
        auto result = PhoneMatcher::searchByNumberPrefix("+381", contacts);
        REQUIRE(result.empty());
    }

    SECTION("empty contacts") {
        std::vector<Contact> contacts;
        auto result = PhoneMatcher::searchByNumberPrefix("+381", contacts);
        REQUIRE(result.empty());
    }
}
