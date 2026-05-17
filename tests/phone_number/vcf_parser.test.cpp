#include <catch2/catch_test_macros.hpp>
#include <phone_number/vcf_parser.hpp>
#include <phone_number/phone_normalizer.hpp>
#include <phone_number/test_helper.hpp>
#include <filesystem>
#include <fstream>
#include <string>

namespace {
class TempVcfFile {
  public:
    explicit TempVcfFile(const std::string &content) {
        static int counter = 0;
        path = std::filesystem::temp_directory_path() /
               ("vcf_parser_test_" + std::to_string(counter++) + ".vcf");
        std::ofstream out(path);
        out << content;
    }
    ~TempVcfFile() {
        std::error_code ec;
        std::filesystem::remove(path, ec);
    }
    std::string filename() const { return path.string(); }

  private:
    std::filesystem::path path;
};

class TempVcfBinaryFile {
  public:
    explicit TempVcfBinaryFile(const std::string &content) {
        static int counter = 0;
        path = std::filesystem::temp_directory_path() /
               ("vcf_parser_binary_test_" + std::to_string(counter++) + ".vcf");
        std::ofstream out(path, std::ios::binary);
        out << content;
    }
    ~TempVcfBinaryFile() {
        std::error_code ec;
        std::filesystem::remove(path, ec);
    }
    std::string filename() const { return path.string(); }

  private:
    std::filesystem::path path;
};
} 

TEST_CASE("VcfParser loads a single contact", "[VcfParser]") {
    TempMetaFile meta;
    TempVcfFile vcf(
        "BEGIN:VCARD\r\n"
        "VERSION:3.0\r\n"
        "N:Petrovic;Marko;;;\r\n"
        "FN:Marko Petrovic\r\n"
        "TEL:+38166123456\r\n"
        "EMAIL:marko@email.com\r\n"
        "END:VCARD\r\n"
    );

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].getName()    == "Marko");
    REQUIRE(contacts[0].getSurname() == "Petrovic");
    REQUIRE(contacts[0].getEmail()   == "marko@email.com");
    REQUIRE(contacts[0].hasPhoneNumbers());
    REQUIRE(contacts[0].getPrimaryPhoneNumber().getNormalizedValue() == "+38166123456");
}

TEST_CASE("VcfParser loads multiple contacts", "[VcfParser]") {
    TempMetaFile meta;
    TempVcfFile vcf(
        "BEGIN:VCARD\r\n"
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
        "END:VCARD\r\n"
    );

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 2);
    REQUIRE(contacts[0].getName() == "Marko");
    REQUIRE(contacts[1].getName() == "Jovana");
}

TEST_CASE("VcfParser normalizes local phone number", "[VcfParser]") {
    TempMetaFile meta;
    TempVcfFile vcf(
        "BEGIN:VCARD\r\n"
        "VERSION:3.0\r\n"
        "N:Petrovic;Marko;;;\r\n"
        "TEL:066123456\r\n"
        "END:VCARD\r\n"
    );

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].getPrimaryPhoneNumber().getNormalizedValue() == "+38166123456");
}

TEST_CASE("VcfParser loads contact with address", "[VcfParser]") {
    TempMetaFile meta;
    TempVcfFile vcf(
        "BEGIN:VCARD\r\n"
        "VERSION:3.0\r\n"
        "N:Petrovic;Marko;;;\r\n"
        "TEL:+38166123456\r\n"
        "ADR:;;21 Main St;Novi Sad;;10000;Serbia\r\n"
        "END:VCARD\r\n"
    );

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].hasAddress());
    REQUIRE(contacts[0].getAddress().getCity()    == "Novi Sad");
    REQUIRE(contacts[0].getAddress().getCountry() == "Serbia");
    REQUIRE(contacts[0].getAddress().getStreet()  == "21 Main St");
}

TEST_CASE("VcfParser loads contact with multiple phone numbers", "[VcfParser]") {
    TempMetaFile meta;
    TempVcfFile vcf(
        "BEGIN:VCARD\r\n"
        "VERSION:3.0\r\n"
        "N:Petrovic;Marko;;;\r\n"
        "TEL:+38166123456\r\n"
        "TEL:+38163999999\r\n"
        "END:VCARD\r\n"
    );

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].getPhoneNumbers().size() == 2);
}

TEST_CASE("VcfParser handles contact with no phone number", "[VcfParser]") {
    TempMetaFile meta;
    TempVcfFile vcf(
        "BEGIN:VCARD\r\n"
        "VERSION:3.0\r\n"
        "N:Petrovic;Marko;;;\r\n"
        "EMAIL:marko@email.com\r\n"
        "END:VCARD\r\n"
    );

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE_FALSE(contacts[0].hasPhoneNumbers());
    REQUIRE(contacts[0].getEmail() == "marko@email.com");
}

TEST_CASE("VcfParser handles contact with no email", "[VcfParser]") {
    TempMetaFile meta;
    TempVcfFile vcf(
        "BEGIN:VCARD\r\n"
        "VERSION:3.0\r\n"
        "N:Petrovic;Marko;;;\r\n"
        "TEL:+38166123456\r\n"
        "END:VCARD\r\n"
    );

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].getEmail().empty());
}

TEST_CASE("VcfParser returns empty vector for missing file", "[VcfParser]") {
    TempMetaFile meta;
    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile("fileDoesNotExist.vcf");

    REQUIRE(contacts.empty());
}

TEST_CASE("VcfParser returns empty vector for empty file", "[VcfParser]") {
    TempMetaFile meta;
    TempVcfFile vcf("");

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.empty());
}

TEST_CASE("VcfParser handles contact with only name, no other fields", "[VcfParser]") {
    TempMetaFile meta;
    TempVcfFile vcf(
        "BEGIN:VCARD\r\n"
        "VERSION:3.0\r\n"
        "N:Petrovic;Marko;;;\r\n"
        "END:VCARD\r\n"
    );

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].getName()    == "Marko");
    REQUIRE(contacts[0].getSurname() == "Petrovic");
    REQUIRE_FALSE(contacts[0].hasPhoneNumbers());
    REQUIRE_FALSE(contacts[0].hasAddress());
    REQUIRE(contacts[0].getEmail().empty());
}

TEST_CASE("VcfParser uses correct region for normalization", "[VcfParser]") {
    TempMetaFile meta;
    TempVcfFile vcf(
        "BEGIN:VCARD\r\n"
        "VERSION:3.0\r\n"
        "N:Smith;John;;;\r\n"
        "TEL:12125551234\r\n"   
        "END:VCARD\r\n"
    );

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "US");  

    auto contacts = parser.loadFromFile(vcf.filename());

    REQUIRE(contacts.size() == 1);
    REQUIRE(contacts[0].getPrimaryPhoneNumber().getNormalizedValue() == "+12125551234");
}

TEST_CASE("VcfParser handles malformed vCard — missing END", "[VcfParser]") {
    TempMetaFile meta;
    TempVcfFile vcf(
        "BEGIN:VCARD\r\n"
        "VERSION:3.0\r\n"
        "N:Petrovic;Marko;;;\r\n"
        "TEL:+38166123456\r\n"
    );

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");

    REQUIRE_NOTHROW(parser.loadFromFile(vcf.filename()));
}

TEST_CASE("VcfParser handles malformed vCard — missing BEGIN", "[VcfParser]") {
    TempMetaFile meta;
    TempVcfFile vcf(
        "VERSION:3.0\r\n"
        "N:Petrovic;Marko;;;\r\n"
        "TEL:+38166123456\r\n"
        "END:VCARD\r\n"
    );

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");

    REQUIRE_NOTHROW(parser.loadFromFile(vcf.filename()));
}

TEST_CASE("VcfParser handles malformed vCard — garbage content", "[VcfParser]") {
    TempMetaFile meta;
    TempVcfFile vcf("this is not a vcard file at all!!!\nrandom garbage\n12345\n");

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");

    REQUIRE_NOTHROW(parser.loadFromFile(vcf.filename()));
}

TEST_CASE("VcfParser handles contact with photo — binary data does not break parsing", "[VcfParser]") {
    TempMetaFile meta;

    TempVcfBinaryFile vcf(
        "BEGIN:VCARD\r\n"
        "VERSION:3.0\r\n"
        "N:Petrovic;Marko;;;\r\n"
        "TEL:+38166123456\r\n"
        "PHOTO;ENCODING=b;TYPE=JPEG:/9j/4AAQSkZJRgABAQEASABIAAD/2wBDAAgGBgcGBQgH\r\n"
        " BwcJCQgKDBQNDAsLDBkSEw8UHRofHh0aHBwgJC4nICIsIxwcKDcpLDAxNDQ0Hyc5PTgy\r\n"
        " \r\n"
        "EMAIL:marko@email.com\r\n"
        "END:VCARD\r\n"
    );

    PhoneNormalizer normalizer(meta.filename());
    VcfParser parser(normalizer, "RS");
    std::vector<Contact> contacts;

    REQUIRE_NOTHROW(contacts = parser.loadFromFile(vcf.filename()));
    
    if (!contacts.empty()) {
        REQUIRE(contacts[0].getName() == "Marko");
        REQUIRE(contacts[0].getSurname() == "Petrovic");
        REQUIRE(contacts[0].getPrimaryPhoneNumber().getNormalizedValue() == "+38166123456");
    }
}

