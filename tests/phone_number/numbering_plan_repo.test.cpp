#include <catch2/catch_test_macros.hpp>
#include <phone_number/numbering_plan_repo.hpp>
#include <filesystem>
#include <fstream>
#include <string>

namespace {
    class TempJsonFile {
        public:
            explicit TempJsonFile(const std::string& content) {
                static int counter = 0;

                path = std::filesystem::temp_directory_path() /
                    ("numbering_plan_repo_test_" + std::to_string(counter++) + ".json");

                std::ofstream out(path);
                out << content;
            }

            ~TempJsonFile() {
                std::error_code ec;
                std::filesystem::remove(path, ec);
            }

            std::string filename() const {
                return path.string();
            }

        private:
            std::filesystem::path path;
    };
}

TEST_CASE("loadPlans loads from JSON", "[NumberingPlanRepo]") {
      TempJsonFile file(R"json(
        {
            "countries":{
                "RS": ["381", "00", "0"],
                "US": ["1", "011", "1"],
                "DE": ["49", "00", "0"]
            }
        }
    )json");

    NumberingPlanRepo repo;
    repo.loadPlans(file.filename());

    SECTION("lookup by ISO country works"){
        const NumberingPlan& rs = repo.getForIsoCountry("RS");

        REQUIRE(rs.isoCountryName == "RS");
        REQUIRE(rs.countryCode == 381);
        REQUIRE(rs.internationalPrefix == "00");
        REQUIRE(rs.nationalPrefix == "0");
    }
    SECTION("unknown ISO country returns empty plan") {
        const NumberingPlan& plan = repo.getForIsoCountry("FR");

        REQUIRE(plan.countryCode == -1);
        REQUIRE(plan.isoCountryName.empty());
        REQUIRE(plan.nationalPrefix.empty());
        REQUIRE(plan.internationalPrefix.empty());
    }

    SECTION("lookup by country code works"){
        const NumberingPlan& us = repo.getForCountryCode(1);

        REQUIRE(us.isoCountryName == "US");
        REQUIRE(us.countryCode == 1);
        REQUIRE(us.internationalPrefix == "011");
        REQUIRE(us.nationalPrefix == "1");
    }
    SECTION("unknown country code returns empty plan") {
        const NumberingPlan& plan = repo.getForCountryCode(999);

        REQUIRE(plan.countryCode == -1);
        REQUIRE(plan.isoCountryName.empty());
        REQUIRE(plan.nationalPrefix.empty());
        REQUIRE(plan.internationalPrefix.empty());
    }

    SECTION("country code existence check works") {
        REQUIRE(repo.doesCountryCodeExist(381));
        REQUIRE(repo.doesCountryCodeExist(1));
        REQUIRE(repo.doesCountryCodeExist(49));

        REQUIRE_FALSE(repo.doesCountryCodeExist(33));
        REQUIRE_FALSE(repo.doesCountryCodeExist(999));
    }

    SECTION("international prefix existance check works") {
        REQUIRE(repo.doesInternationalPrefixExist("00"));
        REQUIRE(repo.doesInternationalPrefixExist("011"));
     
        REQUIRE_FALSE(repo.doesInternationalPrefixExist("001"));
        REQUIRE_FALSE(repo.doesInternationalPrefixExist("99"));
    }
}

TEST_CASE("NumberingPlanRepo clears old plans when loading again", "[NumberingPlanRepo]"){
    TempJsonFile firstFile(R"json(
        {
            "countries": {
                "RS": ["381", "00", "0"]
            }
        }
    )json");

    TempJsonFile secondFile(R"json(
        {
            "countries": {
                "US": ["1", "011", "1"]
            }
        }
    )json");

    NumberingPlanRepo repo;

    repo.loadPlans(firstFile.filename());
    
    REQUIRE(repo.doesCountryCodeExist(381));
    REQUIRE(repo.getForIsoCountry("RS").countryCode == 381);

    repo.loadPlans(secondFile.filename());

    REQUIRE_FALSE(repo.doesCountryCodeExist(381));
    const NumberingPlan& plan = repo.getForIsoCountry("RS");
    REQUIRE(plan.countryCode == -1);
    REQUIRE(plan.isoCountryName.empty());
    REQUIRE(plan.nationalPrefix.empty());
    REQUIRE(plan.internationalPrefix.empty());

    REQUIRE(repo.doesCountryCodeExist(1));
    REQUIRE(repo.getForIsoCountry("US").countryCode == 1);
}

TEST_CASE("loadPlans with missing file return empty repo", "[NumberingPlanRepo]"){
    TempJsonFile file(R"json(
        {
            "countries": {
                "RS": ["381", "00", "0"]
            }
        }
    )json");

    NumberingPlanRepo repo;
    repo.loadPlans(file.filename());

    REQUIRE(repo.doesCountryCodeExist(381));

    repo.loadPlans("fileDoesNotExist.json");

    REQUIRE_FALSE(repo.doesCountryCodeExist(381));
    const NumberingPlan& plan = repo.getForIsoCountry("RS");
    REQUIRE(plan.countryCode == -1);
    REQUIRE(plan.isoCountryName.empty());
    REQUIRE(plan.nationalPrefix.empty());
    REQUIRE(plan.internationalPrefix.empty());
}