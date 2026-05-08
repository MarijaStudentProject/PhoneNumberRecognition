#include "phone-normilizer.hpp"
#include <iostream>

bool PhoneNormalizer::tryParseCountryCode(const std::string &phoneNumber, int &countryCode, std::string &nationalNumber) {
    int currentCountryCode = 0;
    for (int i = 0; i < 3; i++)
    {
        currentCountryCode = currentCountryCode * 10 + (phoneNumber[i] - '0');

        if (repo.doesCountryCodeExist(currentCountryCode)) {
            countryCode = currentCountryCode;
            nationalNumber = phoneNumber.substr(i + 1);
            return true;
        }
    }

    return false;
}

bool PhoneNormalizer::tryParseInternationalPrefix(const std::string &phoneNumber, std::string &truncNumber) {
    if (phoneNumber[0] == '+') {
        truncNumber = phoneNumber.substr(1);
        return true;
    }
    
    //TODO try other international prefixes 

    return false;
}

std::string PhoneNormalizer::normalize(const std::string &phoneNumber) {
    return normalize(phoneNumber, defaultCountryIso);
}

// phone number is already cleaned and only contains digits
std::string PhoneNormalizer::normalize(const std::string &phoneNumber, std::string assumedIsoCountry) {
    NumberingPlan plan;

    int countryCode;
    std::string normalizedNumber;
    std::string nationalNumber;

    std::string truncNumber;
    if (tryParseInternationalPrefix(phoneNumber, truncNumber)) {
        if(tryParseCountryCode(truncNumber, countryCode, nationalNumber)) { //TODO optimize by not creating substrings
            plan = repo.getForCountryCode(countryCode);
            normalizedNumber = plan.internationalPrefix + nationalNumber;
            return normalizedNumber;
        }
        else {
            std::cerr << "Failed to parse country code \n";
            return "";
        }
        
    }
    else {
        plan = repo.getForIsoCountry(assumedIsoCountry);
        
        if (phoneNumber.substr(0, plan.nationalPrefix.size()) == plan.nationalPrefix) {
            nationalNumber = phoneNumber.substr(plan.nationalPrefix.size());
        
            normalizedNumber = "+" + plan.countryCode + nationalNumber;        
        }
        else {
            std::cerr << "Failed to parse national number \n";
            return "";
        }
    }
    return std::string();
}