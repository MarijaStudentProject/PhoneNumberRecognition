#ifndef NUMBERING_PLAN_HPP
#define NUMBERING_PLAN_HPP

#include <string>

struct NumberingPlan {
    int countryCode = -1;            // 381 for Serbia, 49 for Germany, 1 for USA, etc
    std::string isoCountryName;      // RS for Serbia, DE for Germany, US for USA, etc
    std::string nationalPrefix;      // 0 for Serbia, 8 for Russia, 1 for USA, etc,
                                       // used to remove national prefix when we have number in local format
                                       // for example (0)66 555 555 -> +381 66 555 555
    std::string internationalPrefix; // 00 for Serbia, 011 for USA, etc
                                       // used to remove international prefix when we have international number without
                                       // + for example 00381 66 555 555 -> +381 66 555
};

#endif
