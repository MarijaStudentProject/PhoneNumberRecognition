#include <catch2/catch_test_macros.hpp>
#include "phone_number/phone_normalizer.hpp"
#include <iostream>
#include <string_view>
#include <utility>
#include <filesystem>
#include <fstream>
#include <string>
namespace{
class TempMetaFile {
  public:
    TempMetaFile() {
        static int counter = 0;
        path = std::filesystem::temp_directory_path() /
               ("vcf_meta_test_" + std::to_string(counter++) + ".json");
        std::ofstream out(path);
        out << R"json({
            "countries": {
                "RS":["381","00","0"],
                "US": ["1",   "011", "1"],
                "DE": ["49",  "00",  "0"],
                "IT": ["39",  "00",  "0"]
            }
        })json";
    }
    ~TempMetaFile() {
        std::error_code ec;
        std::filesystem::remove(path, ec);
    }
    std::string filename() const { return path.string(); }

  private:
    std::filesystem::path path;
    };
} 


TEST_CASE("normalizer international prefix variants", "[normalizer]") {
    TempMetaFile meta;
    PhoneNormalizer normalizer(meta.filename());
    // non strict mode, country code known
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
    // non strict mode, country code unknown
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

TEST_CASE("normalizer input formatting", "[normalizer]") {
    TempMetaFile meta;
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
}

TEST_CASE("normalizer strict mode", "[normalizer]") {
    TempMetaFile meta;
    PhoneNormalizer normalizer(meta.filename());

    
    SECTION("Serbian local number with RS iso") {
        auto result = normalizer.normalize("066 1234567", "RS", true);
        REQUIRE(result.getNormalizedValue() == "+381661234567");
        REQUIRE(result.getCountryCode() == 381); 
        REQUIRE(result.getNationalNumber() == "661234567");
    }
    SECTION("local fixed line prefix") {
        auto result = normalizer.normalize("011 66 555 555", "RS", true);
        REQUIRE(result.getNormalizedValue() == "+3811166555555");
        REQUIRE(result.getCountryCode() == 381); 

    }
    SECTION("italy - edge case") {
        REQUIRE(normalizer.normalize("066 555 555", "IT", true).getNormalizedValue() == "+39066555555");
    } 
    
    SECTION("local number with wrong country") {
        auto result = normalizer.normalize("066123456", "US", true);
        REQUIRE(result.getRawValue() == "066123456");
        REQUIRE(result.getCountryCode() == 0);
        REQUIRE(result.getNationalNumber() == "");
    }
    
    SECTION("international number still works in strict mode") {
        auto result = normalizer.normalize("+38166123456", "RS", true);
        REQUIRE(result.getCountryCode() == 381);
    }
}

TEST_CASE("normalizer fallback behavior", "[normalizer]") {
    TempMetaFile meta;
    bool strict = true;
    PhoneNormalizer normalizer(meta.filename());

    SECTION("unrecognizable number returns input as is") {
        auto result = normalizer.normalize("555");
        REQUIRE(result.getRawValue() == "555");
        REQUIRE(result.getCountryCode() == 0);
        REQUIRE(result.getNationalNumber() == "");
    }

    SECTION("empty string") {
        auto result = normalizer.normalize("");
        REQUIRE(result.getRawValue() == "");
        REQUIRE(result.getCountryCode() == 0);
        REQUIRE(result.getNationalNumber() == "");
    }

    SECTION("only letters") {
        auto result = normalizer.normalize("abc");
        REQUIRE(result.getCountryCode() == 0);
        REQUIRE(result.getNationalNumber() == "");
    }

    SECTION("number too short but it recognizes the international prefix given - edge case") {
        auto result = normalizer.normalize("+381");
        REQUIRE(result.getCountryCode() == 381);
        REQUIRE(result.getNationalNumber() == "");
    }
}

TEST_CASE("normalizer local number, non strict mode, origin country unknown - fallback behavior", "[normalizer]") {
    TempMetaFile meta;
    PhoneNormalizer normalizer(meta.filename());
    // with lack of info it returns the same raw number
    SECTION("italy") { 
        auto result = normalizer.normalize("066 555 555", "", false);
        REQUIRE(result.getNormalizedValue() == "066555555");
        REQUIRE(result.getCountryCode() == 0);

    } 
    SECTION("us") { 
        auto result = normalizer.normalize("(312) 555-0199", "", false);
        REQUIRE(result.getNormalizedValue() == "3125550199");
        REQUIRE(result.getCountryCode() == 0);
        
    } 
}