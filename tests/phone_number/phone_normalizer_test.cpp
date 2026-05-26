#include "test_helper.hpp"
#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include "phone_number/phone_normalizer.hpp"
#include "phone_number/phone_matcher.hpp"
#include "phone_number/vcf_parser.hpp"

TEST_CASE("international number - country code known", "[normalizer]") {

    auto meta = makeTempMetaFile();
    PhoneNormalizer normalizer(meta.filename());

    SECTION("+ prefix clean number") {
        auto result = normalizer.normalize("+38166123456", "RS", false);
        REQUIRE(result.getNormalizedValue() == "+38166123456");
        REQUIRE(result.getCountryCode() == 381);
        REQUIRE(result.getNationalNumber() == "66123456");
    }

    SECTION("00 prefix") {
        auto result = normalizer.normalize("0038166123456", "RS", false);
        REQUIRE(result.getNormalizedValue() == "+38166123456");
        REQUIRE(result.getCountryCode() == 381);
        REQUIRE(result.getNationalNumber() == "66123456");
    }

    SECTION("US number") {
        auto result = normalizer.normalize("+15551234567", "US", false);
        REQUIRE(result.getNormalizedValue() == "+15551234567");
        REQUIRE(result.getCountryCode() == 1);
        REQUIRE(result.getNationalNumber() == "5551234567");
    }
}

TEST_CASE("international number - country code unknown", "[normalizer]") {
    auto meta = makeTempMetaFile();
    PhoneNormalizer normalizer(meta.filename());

    SECTION("+ prefix clean number") {
        auto result = normalizer.normalize("+38166123456", "", false);
        REQUIRE(result.getNormalizedValue() == "+38166123456");
        REQUIRE(result.getCountryCode() == 381);
        REQUIRE(result.getNationalNumber() == "66123456");
    }

    SECTION("00 prefix") {
        auto result = normalizer.normalize("0038166123456", "", false);
        REQUIRE(result.getNormalizedValue() == "+38166123456");
        REQUIRE(result.getCountryCode() == 381);
        REQUIRE(result.getNationalNumber() == "66123456");
    }

    SECTION("US number") {
        auto result = normalizer.normalize("+15551234567", "", false);
        REQUIRE(result.getNormalizedValue() == "+15551234567");
        REQUIRE(result.getCountryCode() == 1);
        REQUIRE(result.getNationalNumber() == "5551234567");
    }
}

TEST_CASE("local number", "[normalizer]") {
    auto meta = makeTempMetaFile();
    PhoneNormalizer normalizer(meta.filename());

    SECTION("Serbian local number with RS iso") {
        auto result = normalizer.normalize("066 1234567", "RS", true);
        REQUIRE(result.getNormalizedValue() == "+381661234567");
        REQUIRE(result.getCountryCode() == 381);
        REQUIRE(result.getNationalNumber() == "661234567");
    }
    SECTION("italy - edge case") {
        REQUIRE(normalizer.normalize("066 555 555", "IT", true).getNormalizedValue() == "+39066555555");
    }

    SECTION("local number with wrong country") {
        auto result = normalizer.normalize("066123456", "US", true);
        REQUIRE(result.getRawValue() == "066123456");
        REQUIRE(result.getCountryCode() == 0);
        REQUIRE(result.getNationalNumber().empty());
    }
    SECTION("local number starting with international prefix but no ambiguouity") {
        // strictness doesnt make a difference here because a national prefix with +69 doesnt exist
        auto result1 = normalizer.normalize("011 69 3333", "RS", true);
        auto result2 = normalizer.normalize("011 69 3333", "RS", false);

        REQUIRE(result1.getNormalizedValue() == "+38111693333");
        REQUIRE(result2.getNormalizedValue() == "+38111693333");
    }
    SECTION("local fixed line prefix strict origin") {
        auto result = normalizer.normalize("011 66 555 555", "RS", true);
        REQUIRE(result.getNormalizedValue() == "+3811166555555");
        REQUIRE(result.getCountryCode() == 381);
    }
    SECTION("local fixed line prefix - non strict matches us international") {
        auto result = normalizer.normalize("011 66 555 555", "RS", false);
        REQUIRE(result.getNormalizedValue() == "+66555555");
        REQUIRE(result.getCountryCode() == 66);
    }

    SECTION("international number works in strict mode") {
        auto result = normalizer.normalize("+38166123456", "RS", true);
        REQUIRE(result.getCountryCode() == 381);
    }
}

TEST_CASE("local number no country", "[normalizer]") {
    auto meta = makeTempMetaFile();
    PhoneNormalizer normalizer(meta.filename());

    SECTION("ambiguous local number italy or serbia") {
        auto result = normalizer.normalize("066 555 555", "", false);
        REQUIRE(result.getRawValue() == "066555555");
        REQUIRE(result.getCountryCode() == 0);
    }
    SECTION("ambigous local number us") {
        auto result = normalizer.normalize("(312) 555-0199", "", false);
        REQUIRE(result.getRawValue() == "3125550199");
        REQUIRE(result.getCountryCode() == 0);
    }
}

TEST_CASE("normalizer fallback behavior", "[normalizer]") {
    auto meta = makeTempMetaFile();
    bool strict = true;
    PhoneNormalizer normalizer(meta.filename());

    SECTION("unrecognizable number returns input as is") {
        auto result = normalizer.normalize("555");
        REQUIRE(result.getRawValue() == "555");
        REQUIRE(result.getCountryCode() == 0);
        REQUIRE(result.getNationalNumber().empty());
    }

    SECTION("empty string") {
        auto result = normalizer.normalize("");
        REQUIRE(result.getRawValue().empty());
        REQUIRE(result.getCountryCode() == 0);
        REQUIRE(result.getNationalNumber().empty());
    }

    SECTION("only letters") {
        auto result = normalizer.normalize("abc");
        REQUIRE(result.getCountryCode() == 0);
        REQUIRE(result.getNationalNumber().empty());
    }

    SECTION("number too short but it recognizes the international prefix and the international prefix is found - edge "
            "case") {
        auto result = normalizer.normalize("+381");
        REQUIRE(result.getCountryCode() == 381);
        REQUIRE(result.getNationalNumber().empty());
    }
    SECTION("unknown international prefix - edge case") {
        // treated as unknown number
        auto result = normalizer.normalize("+69 33333");
        REQUIRE(result.getCountryCode() == 0);
        REQUIRE(result.getNationalNumber().empty());
        REQUIRE(result.getNormalizedValue().empty());
    }
}

TEST_CASE("normalizer input formatting", "[normalizer]") {
    auto meta = makeTempMetaFile();
    PhoneNormalizer normalizer(meta.filename());

    SECTION("strips spaces") {
        auto result = normalizer.normalize("+381 66 123 456");
        REQUIRE(result.getNormalizedValue() == "+38166123456");
    }

    SECTION("strips dashes") {
        auto result = normalizer.normalize("+381-66-123-456");
        REQUIRE(result.getNormalizedValue() == "+38166123456");
    }

    SECTION("strips parentheses") {
        auto result = normalizer.normalize("+381(66)123456");
        REQUIRE(result.getNormalizedValue() == "+38166123456");
    }

    SECTION("idempotent") {
        auto result = normalizer.normalize("+38166123456");
        REQUIRE(result.getNormalizedValue() == "+38166123456");
    }
    //
    SECTION("+ is not the first character") {
        auto result = normalizer.normalize("381+66123456");
        REQUIRE(result.getRawValue() == "38166123456");
    }
}

TEST_CASE("normalizer integration test"){
    PhoneNormalizer normalizer("../resources/min_filtered_metadata.json");
    VcfParser parser(normalizer, "RS");
    std::vector<Contact> contacts = parser.loadFromFile("../resources/contacts_100.vcf");

    auto nLocal=normalizer.normalize("060 1231008", "RS", false);
    auto nInt00=normalizer.normalize("00 381 60 1231008", "RS", false);
    auto nIntPlus=normalizer.normalize("+ 381 60 1231008", "RS", false);
    auto nInvalid=normalizer.normalize("00 060 1231008", "RS", false);

    REQUIRE(nLocal.getNormalizedValue()=="+381601231008");
    REQUIRE(nInt00.getNormalizedValue()=="+381601231008");
    REQUIRE(nIntPlus.getNormalizedValue()=="+381601231008");
    REQUIRE(nInvalid.getNormalizedValue()!="+381601231008");

    auto m1=PhoneMatcher::findMatch(nLocal, contacts);
    auto m2=PhoneMatcher::findMatch(nInt00, contacts);
    auto m3=PhoneMatcher::findMatch(nIntPlus, contacts);
    
    REQUIRE(m1);
    REQUIRE(m2);
    REQUIRE(m3);

    REQUIRE(contacts[m1.value()].getName()=="Aleksa");
    REQUIRE(contacts[m1.value()].getSurname()=="Djordjevic");
    REQUIRE(m1.value()==m2.value());
    REQUIRE(m1.value()==m3.value());
    

}