#include "phone_number/numbering_plan_repo.hpp"
#define JSMN_STATIC
#include "../../libs/jsmn/jsmn.h"
#include <cstring>
#include <fstream>
#include <iostream>

void NumberingPlanRepo::load(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return;
    }

    // read file
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const char *js = content.c_str();
    std::size_t len = content.size();

    // tokenize it
    jsmntok_t tokens[4096];
    jsmn_parser parser;
    jsmn_init(&parser);
    if (jsmn_parse(&parser, js, len, tokens, 4096) < 0) {
        std::cerr << "Failed to parse JSON\n";
        return;
    }

    auto tokstr = [](const char *json, const jsmntok_t *tok) { // helper to convert to string
        return std::string(json + tok->start, tok->end - tok->start);
    };

    // we skip tokens[0]=root obj, tokens[1]="countries", tokens[2]=countries obj

    int num_countries = tokens[2].size; // token of array, number of coutry entries
    plans.reserve(num_countries);

    // Iso key: array token callingCode, intlPrefix, natPrefix
    int i = 3;
    for (int c = 0; c < num_countries;
         c++, i += 5) { // stride 5: array token, iso key, country code, intl prefix, nat prefix
        std::string isoCode = tokstr(js, &tokens[i]);
        int countryCode = std::stoi(tokstr(js, &tokens[i + 2]));
        std::string intlPrefix = tokstr(js, &tokens[i + 3]);
        std::string nationalPrefix = tokstr(js, &tokens[i + 4]);


        NumberingPlan p{countryCode, isoCode, nationalPrefix, intlPrefix};
        plans.push_back(p);
        isoPlanMap[p.isoCountryName] = &plans.back();
        countryCodePlanMap[p.countryCode] = &plans.back();
        internationalPrefixSet.insert(p.internationalPrefix);
    }
}

NumberingPlan NumberingPlanRepo::getForIsoCountry(const std::string &isoCountry) const {
    if (isoPlanMap.find(isoCountry) == isoPlanMap.end()) {
        return NumberingPlan();
    }
    return *isoPlanMap.at(isoCountry);
}

bool NumberingPlanRepo::doesCountryCodeExist(int countryCode) const {
    return countryCodePlanMap.find(countryCode) != countryCodePlanMap.end();
}

bool NumberingPlanRepo::doesInternationalPrefixExist(const std::string& internationalPrefix) const {
    return internationalPrefixSet.find(internationalPrefix) != internationalPrefixSet.end();
}

NumberingPlan NumberingPlanRepo::getForCountryCode(int countryCode) const {
    if (countryCodePlanMap.find(countryCode) == countryCodePlanMap.end()) {
        return NumberingPlan();
    }
    return *countryCodePlanMap.at(countryCode);
}
