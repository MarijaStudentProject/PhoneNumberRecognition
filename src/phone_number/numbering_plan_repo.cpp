#include "phone_number/numbering_plan_repo.hpp"
#define JSMN_STATIC
#include "../../libs/jsmn/jsmn.h"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>

void NumberingPlanRepo::loadPlans(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
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
    if (jsmn_parse(&parser, js, len, tokens.data(), tokens.size()) < 0) {
        std::cerr << "Failed to parse JSON\n";
        return;
    }

    auto tokstr = [&jsView](const jsmntok_t &tok) {
        return std::string(jsView.substr(tok.start, tok.end - tok.start));
    };

    // we skip tokens[0]=root obj, tokens[1]="countries", tokens[2]=countries obj
    int numCountries = tokens.at(2).size; // token of array, number of country entries
    m_plans.reserve((numCountries));

    // Iso key: array token callingCode, intlPrefix, natPrefix
    int i = 3;
    for (int c = 0; c < numCountries;
         c++, i += 5) { // stride 5: array token, iso key, country code, intl prefix, nat prefix
        auto idx = i;
        std::string isoCode = tokstr(tokens.at(idx));
        int countryCode = std::stoi(tokstr(tokens.at(idx + 2)));
        std::string intlPrefix = tokstr(tokens.at(idx + 3));
        std::string natPrefix = tokstr(tokens.at(idx + 4));

        NumberingPlan p{countryCode, isoCode, natPrefix, intlPrefix};
        m_plans.push_back(p);
        m_isoPlanMap[p.m_isoCountryName] = &m_plans.back();
        m_countryCodePlanMap[p.m_countryCode] = &m_plans.back();
        m_internationalPrefixSet.insert(p.m_internationalPrefix);
    }
}

NumberingPlan NumberingPlanRepo::getForIsoCountry(const std::string &isoCountry) const {
    if (m_isoPlanMap.find(isoCountry) == m_isoPlanMap.end()) {
        return {};
    }
    return *m_isoPlanMap.at(isoCountry);
}

bool NumberingPlanRepo::doesCountryCodeExist(int countryCode) const {
    return m_countryCodePlanMap.find(countryCode) != m_countryCodePlanMap.end();
}

bool NumberingPlanRepo::doesInternationalPrefixExist(const std::string &internationalPrefix) const {
    return m_internationalPrefixSet.find(internationalPrefix) != m_internationalPrefixSet.end();
}

NumberingPlan NumberingPlanRepo::getForCountryCode(int countryCode) const {
    if (m_countryCodePlanMap.find(countryCode) == m_countryCodePlanMap.end()) {
        return {};
    }
    return *m_countryCodePlanMap.at(countryCode);
}
