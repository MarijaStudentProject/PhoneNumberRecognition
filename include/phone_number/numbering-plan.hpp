#ifndef NUMBERING_PLAN_HPP
#define NUMBERING_PLAN_HPP

#include <string>

struct NumberingPlan {
    int countryCode;
    std::string isoCountryName; 
    std::string nationalPrefix;
    std::string internationalPrefix;
};

#endif 
