#ifndef PHONE_NORMALIZER_HPP
#define PHONE_NORMALIZER_HPP

#include "numbering-plan.hpp"
#include "numbering-plan-repo.hpp"
#include <string>

class PhoneNormalizer {
private:
    // fallback country, set to phone owner's country by default  
    std::string defaultCountryIso = "RS"; 
    NumberingPlanRepo repo;

    bool tryParseCountryCode(const std::string& phoneNumber, int& countryCode, std::string& nationalNumber);
    bool tryParseInternationalPrefix(const std::string& phoneNumber, std::string& truncNumber, int n);
public:
    std::string normalize(const std::string& phoneNumber);
    std::string normalize(const std::string& phoneNumber, std::string isoCountry);
    PhoneNormalizer() = default;
    PhoneNormalizer(std::string isoCountry) : defaultCountryIso(isoCountry) {}
};

#endif