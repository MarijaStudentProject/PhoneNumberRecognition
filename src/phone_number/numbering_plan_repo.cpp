#include "phone_number/numbering_plan_repo.hpp"
#define JSMN_STATIC
#include <array>
#include <charconv>
#include <cstring>
#include <fstream>
#include <iostream>
#include <jsmn/jsmn.h>
#include <string_view>

NumberingPlanRepo::NumberingPlanRepo(const std::vector<NumberingPlan> &plans) { parsePlans(plans); }

void NumberingPlanRepo::parsePlans(const std::vector<NumberingPlan> &plans) {
    m_isoPlanMap.clear();
    m_countryCodePlanMap.clear();
    m_internationalPrefixSet.clear();

    for (const auto &p : plans) {
        m_isoPlanMap[p.isoCountryName] = p;
        m_countryCodePlanMap[p.countryCode] = p;
        m_internationalPrefixSet.insert(p.internationalPrefix);
    }
}

void NumberingPlanRepo::loadPlans(const std::string &filename) {
    m_isoPlanMap.clear();
    m_countryCodePlanMap.clear();
    m_internationalPrefixSet.clear();

    std::vector<NumberingPlan> plans;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << "\n";
        return;
    }

    // read file
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const char *js = content.c_str();
    std::size_t len = content.size();
    std::string_view jsView(js, len);

    std::array<jsmntok_t, 4096> tokens{};
    // tokenize it
    jsmn_parser parser;
    jsmn_init(&parser);
    int numTokensParsed = jsmn_parse(&parser, js, len, tokens.data(), tokens.size());
    if (numTokensParsed < 0) {
        std::cerr << "Failed to parse JSON in '" << filename << "': error code " << numTokensParsed << "\n";
        return;
    }

    // Validate minimum structure: tokens[0]=root obj, tokens[1]="countries" key, tokens[2]=countries obj
    if (numTokensParsed < 3 || tokens[0].type != JSMN_OBJECT || tokens[2].type != JSMN_OBJECT) {
        std::cerr << "Unexpected JSON structure in '" << filename << "'\n";
        return;
    }

    auto tokstr = [&jsView](const jsmntok_t &tok) {
        return std::string(jsView.substr(tok.start, tok.end - tok.start));
    };

    // we skip tokens[0]=root obj, tokens[1]="countries", tokens[2]=countries obj
    int numCountries = tokens.at(2).size; // token of array, number of country entries

    // Iso key: array token callingCode, intlPrefix, natPrefix
    int i = 3;
    for (int c = 0; c < numCountries;
         c++, i += 5) { // stride 5: iso key, array token (skipped), country code, intl prefix, nat prefix
        // Validate we have enough tokens for this entry
        if (i + 4 >= numTokensParsed) {
            std::cerr << "Truncated JSON in '" << filename << "': expected " << numCountries
                      << " countries but ran out of tokens at entry " << c << "\n";
            break;
        }

        auto idx = i;
        std::string isoCode = tokstr(tokens.at(idx));

        // Use from_chars for safe, non-throwing country code parsing
        std::string ccStr = tokstr(tokens.at(idx + 2));
        int countryCode = 0;
        auto result = std::from_chars(ccStr.data(), ccStr.data() + ccStr.size(), countryCode); // NOLINT
        if (result.ec != std::errc{}) {
            std::cerr << "Invalid country code '" << ccStr << "' for '" << isoCode << "' in '" << filename
                      << "', skipping entry\n";
            continue;
        }

        std::string intlPrefix = tokstr(tokens.at(idx + 3));
        std::string natPrefix = tokstr(tokens.at(idx + 4));

        NumberingPlan p{countryCode, isoCode, natPrefix, intlPrefix};
        plans.push_back(p);
    }
    parsePlans(plans);
}

const NumberingPlan &NumberingPlanRepo::getForIsoCountry(const std::string &isoCountry) const {
    if (m_isoPlanMap.find(isoCountry) == m_isoPlanMap.end()) {
        return EmptyPlan;
    }
    return m_isoPlanMap.at(isoCountry);
}

bool NumberingPlanRepo::doesCountryCodeExist(int countryCode) const {
    return m_countryCodePlanMap.find(countryCode) != m_countryCodePlanMap.end();
}

bool NumberingPlanRepo::doesInternationalPrefixExist(const std::string &internationalPrefix) const {
    return m_internationalPrefixSet.find(internationalPrefix) != m_internationalPrefixSet.end();
}

const NumberingPlan &NumberingPlanRepo::getForCountryCode(int countryCode) const {
    if (m_countryCodePlanMap.find(countryCode) == m_countryCodePlanMap.end()) {
        return EmptyPlan;
    }
    return m_countryCodePlanMap.at(countryCode);
}
