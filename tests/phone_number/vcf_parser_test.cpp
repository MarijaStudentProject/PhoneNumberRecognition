#include "test_helper.hpp"
#include <catch2/catch_test_macros.hpp>
#include <phone_number/contact.hpp>
#include <phone_number/vcf_parser.hpp>

TEST_CASE("VcfParser loads a single contact", "[VcfParser]") {
    TempFile vcf("BEGIN:VCARD\r\n"
                 "VERSION:3.0\r\n"
                 "N:Petrovic;Marko;;;\r\n"
                 "FN:Marko Petrovic\r\n"
                 "TEL:+38166123456\r\n"
                 "EMAIL:marko@email.com\r\n"
                 "END:VCARD\r\n",
                 ".vcf");

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].getName() == "Marko");
    REQUIRE(contacts[0].getSurname() == "Petrovic");
    REQUIRE(contacts[0].getEmail() == "marko@email.com");
    REQUIRE(contacts[0].hasPhoneNumbers());
    REQUIRE(contacts[0].getPrimaryPhoneNumber().getNormalizedValue() == "+38166123456");
}

TEST_CASE("VcfParser loads multiple contacts", "[VcfParser]") {
    TempFile vcf("BEGIN:VCARD\r\n"
                 "VERSION:3.0\r\n"
                 "N:Petrovic;Marko;;;\r\n"
                 "FN:Marko Petrovic\r\n"
                 "TEL:+38166123456\r\n"
                 "END:VCARD\r\n"
                 "BEGIN:VCARD\r\n"
                 "VERSION:3.0\r\n"
                 "N:Jovic;Jovana;;;\r\n"
                 "FN:Jovana Jovic\r\n"
                 "TEL:+38166999999\r\n"
                 "END:VCARD\r\n",
                 ".vcf");

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 2);
    REQUIRE(contacts[0].getName() == "Marko");
    REQUIRE(contacts[1].getName() == "Jovana");
}

TEST_CASE("VcfParser normalizes local phone number", "[VcfParser]") {
    TempFile vcf("BEGIN:VCARD\r\n"
                 "VERSION:3.0\r\n"
                 "N:Petrovic;Marko;;;\r\n"
                 "TEL:066123456\r\n"
                 "END:VCARD\r\n",
                 ".vcf");

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].getPrimaryPhoneNumber().getNormalizedValue() == "+38166123456");
}

TEST_CASE("VcfParser loads contact with address", "[VcfParser]") {
    TempFile vcf("BEGIN:VCARD\r\n"
                 "VERSION:3.0\r\n"
                 "N:Petrovic;Marko;;;\r\n"
                 "TEL:+38166123456\r\n"
                 "ADR:;;21 Main St;Novi Sad;;10000;Serbia\r\n"
                 "END:VCARD\r\n",
                 ".vcf");

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].hasAddress());
    REQUIRE(contacts[0].getAddress().getCity() == "Novi Sad");
    REQUIRE(contacts[0].getAddress().getCountry() == "Serbia");
    REQUIRE(contacts[0].getAddress().getStreet() == "21 Main St");
}

TEST_CASE("VcfParser loads contact with multiple phone numbers", "[VcfParser]") {
    TempFile vcf("BEGIN:VCARD\r\n"
                 "VERSION:3.0\r\n"
                 "N:Petrovic;Marko;;;\r\n"
                 "TEL:+38166123456\r\n"
                 "TEL:+38163999999\r\n"
                 "END:VCARD\r\n",
                 ".vcf");

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].getPhoneNumbers().size() == 2);
}

TEST_CASE("VcfParser handles contact with no phone number", "[VcfParser]") {
    TempFile vcf("BEGIN:VCARD\r\n"
                 "VERSION:3.0\r\n"
                 "N:Petrovic;Marko;;;\r\n"
                 "EMAIL:marko@email.com\r\n"
                 "END:VCARD\r\n",
                 ".vcf");

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE_FALSE(contacts[0].hasPhoneNumbers());
    REQUIRE(contacts[0].getEmail() == "marko@email.com");
}

TEST_CASE("VcfParser handles contact with no email", "[VcfParser]") {
    TempFile vcf("BEGIN:VCARD\r\n"
                 "VERSION:3.0\r\n"
                 "N:Petrovic;Marko;;;\r\n"
                 "TEL:+38166123456\r\n"
                 "END:VCARD\r\n",
                 ".vcf");

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].getEmail().empty());
}

TEST_CASE("VcfParser returns empty vector for missing file", "[VcfParser]") {
    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile("fileDoesNotExist.vcf");

    REQUIRE(contacts.empty());
}

TEST_CASE("VcfParser returns empty vector for empty file", "[VcfParser]") {
    TempFile vcf("", ".vcf");

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.empty());
}

TEST_CASE("VcfParser handles contact with only name, no other fields", "[VcfParser]") {
    TempFile vcf("BEGIN:VCARD\r\n"
                 "VERSION:3.0\r\n"
                 "N:Petrovic;Marko;;;\r\n"
                 "END:VCARD\r\n",
                 ".vcf");

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].getName() == "Marko");
    REQUIRE(contacts[0].getSurname() == "Petrovic");
    REQUIRE_FALSE(contacts[0].hasPhoneNumbers());
    REQUIRE_FALSE(contacts[0].hasAddress());
    REQUIRE(contacts[0].getEmail().empty());
}

TEST_CASE("VcfParser uses correct region for normalization", "[VcfParser]") {
    TempFile vcf("BEGIN:VCARD\r\n"
                 "VERSION:3.0\r\n"
                 "N:Smith;John;;;\r\n"
                 "TEL:12125551234\r\n"
                 "END:VCARD\r\n",
                 ".vcf");

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "US");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].getPrimaryPhoneNumber().getNormalizedValue() == "+12125551234");
}

TEST_CASE("VcfParser handles malformed vCard — missing END", "[VcfParser]") {
    TempFile vcf("BEGIN:VCARD\r\n"
                 "VERSION:3.0\r\n"
                 "N:Petrovic;Marko;;;\r\n"
                 "TEL:+38166123456\r\n",
                 ".vcf");

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");

    REQUIRE_NOTHROW(parser.loadFromFile(vcf.filename()));
}

TEST_CASE("VcfParser handles malformed vCard — missing BEGIN", "[VcfParser]") {
    TempFile vcf("VERSION:3.0\r\n"
                 "N:Petrovic;Marko;;;\r\n"
                 "TEL:+38166123456\r\n"
                 "END:VCARD\r\n",
                 ".vcf");

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");

    REQUIRE_NOTHROW(parser.loadFromFile(vcf.filename()));
}

TEST_CASE("VcfParser handles malformed vCard — garbage content", "[VcfParser]") {
    TempFile vcf("this is not a vcard file at all!!!\nrandom garbage\n12345\n", ".vcf");

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");

    REQUIRE_NOTHROW(parser.loadFromFile(vcf.filename()));
}

TEST_CASE("VcfParser handles contact with photo — binary data does not break parsing", "[VcfParser]") {
    TempFile vcf("BEGIN:VCARD\r\n"
                 "VERSION:3.0\r\n"
                 "N:Petrovic;Marko;;;\r\n"
                 "TEL:+38166123456\r\n"
                 "PHOTO;ENCODING=b;TYPE=JPEG:/9j/4AAQSkZJRgABAQEASABIAAD/2wBDAAgGBgcGBQgH\r\n"
                 " BwcJCQgKDBQNDAsLDBkSEw8UHRofHh0aHBwgJC4nICIsIxwcKDcpLDAxNDQ0Hyc5PTgy\r\n"
                 " \r\n"
                 "EMAIL:marko@email.com\r\n"
                 "END:VCARD\r\n",
                 ".vcf", true);

    auto normalizer = makeTestNormalizer();
    VcfParser parser(normalizer, "RS");
    std::vector<Contact> contacts;

    REQUIRE_NOTHROW(contacts = parser.loadFromFile(vcf.filename()));

    if (!contacts.empty()) {
        REQUIRE(contacts[0].getName() == "Marko");
        REQUIRE(contacts[0].getSurname() == "Petrovic");
        REQUIRE(contacts[0].getPrimaryPhoneNumber().getNormalizedValue() == "+38166123456");
    }
}
