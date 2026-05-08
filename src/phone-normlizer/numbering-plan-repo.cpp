#include "numbering-plan-repo.hpp"
#include "../libs/nlohmannjson/json.hpp"
#include <fstream>
#include <iostream> 

using json = nlohmann::json;

void NumberingPlanRepo::load(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return ;
    }

    json j;
    file >> j;

    plans.reserve(j.size());

    for (const auto& item : j)
    {
        NumberingPlan p{
            item["countryCode"].get<int>(),
            item["isoCountryName"].get<std::string>(),
            item["nationalPrefix"].get<std::string>(),
            item["internationalPrefix"].get<std::string>()
        };

        plans.push_back(std::move(p));
        isoPlanMap[plans.back().isoCountryName] = &plans.back();
        countryCodePlanMap[plans.back().countryCode] = &plans.back();
    }
}

NumberingPlan NumberingPlanRepo::getForIsoCountry(const std::string &isoCountry) const {
    return *isoPlanMap.at(isoCountry);
}

bool NumberingPlanRepo::doesCountryCodeExist(int countryCode) const { 
    return countryCodePlanMap.find(countryCode) != countryCodePlanMap.end(); 
}

NumberingPlan NumberingPlanRepo::getForCountryCode(int countryCode) const {
    return *countryCodePlanMap.at(countryCode);
}
