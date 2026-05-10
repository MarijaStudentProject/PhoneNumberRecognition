#include "phone-normilizer.hpp"
#include <iostream>

bool PhoneNormalizer::tryParseCountryCode(const std::string &phoneNumber, int &countryCode, std::string &nationalNumber) {
    int currentCountryCode = 0;
    for (int i = 3; i > 0; i--)
    {
        currentCountryCode = std::stoi(phoneNumber.substr(0,i));

        if (repo.doesCountryCodeExist(currentCountryCode)) {
            countryCode = currentCountryCode;
            nationalNumber = phoneNumber.substr(i);
            return true;
        }
    }

    return false;
}

bool PhoneNormalizer::tryParseInternationalPrefix(const std::string &phoneNumber, std::string &truncNumber, int n) {
    if(repo.doesInternationalPrefixExist(phoneNumber.substr(0, n))){
        truncNumber = phoneNumber.substr(n);
        return true;
    }
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

    if (phoneNumber[0] == '+') {
         return phoneNumber;
    }else{
        int n = 3;
        while(n>0){
            if(tryParseInternationalPrefix(phoneNumber,truncNumber, n)){
                if(tryParseCountryCode(truncNumber, countryCode, nationalNumber)){
                    plan = repo.getForCountryCode(countryCode);
                    normalizedNumber = "+" + truncNumber;
                    return normalizedNumber;
                }else{
                    n--;
                }
            }else{
                n--;
            }
        }
        // We dont have + and internationalPrefix, so it means that we got number in format 066 555 555 (if we are in Serbia,
        // so we need to put default country Code) 
        plan = repo.getForIsoCountry(assumedIsoCountry);
        normalizedNumber = "+" + plan.countryCode + phoneNumber.substr(plan.nationalPrefix.size());
        return normalizedNumber;
    }   
    

    return std::string();
}