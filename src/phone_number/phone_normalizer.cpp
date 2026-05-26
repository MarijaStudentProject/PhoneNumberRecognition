#include "phone_number/phone_normalizer.hpp"
#include <iostream>
#include <string_view>
#include <utility>

namespace {

std::string cleanPhoneNumber(const std::string &phoneNumber) {
    std::string cleaned;
    for (char c : phoneNumber) {
        if ((isdigit(c) != 0) || c == '+' && cleaned.empty()) {
            cleaned += c;
        }
    }
    return cleaned;
}
bool startsWith(std::string_view str, std::string_view prefix) { return str.substr(0, prefix.size()) == prefix; }
} // namespace

bool PhoneNormalizer::tryParseAnyCountryCode(std::string_view phoneNumber, int &countryCode,
                                             std::string_view &nationalNumber) const {
    int currentCountryCode = 0;
    for (int n = 3; n > 0; n--) {
        if (phoneNumber.size() < n) {
            continue;
        }
        try {
            currentCountryCode = std::stoi(std::string(phoneNumber.substr(0, n)));
        } catch (const std::exception &e) {
            continue; // this should not happen we already cleaned the phone to contain only digits
        }

        if (m_repo.doesCountryCodeExist(currentCountryCode)) {
            countryCode = currentCountryCode;
            nationalNumber = phoneNumber.substr(n);
            return true;
        }
    }

    return false;
}

// On success, sets truncNumber to the portion after the prefix and returns true
bool PhoneNormalizer::tryParseAnyInternationalPrefix(std::string_view phoneNumber,
                                                     std::string_view &truncNumber) const {
    for (int n = 4; n > 0; n--) { // start from longest 0011 australia first then european 00

        if (m_repo.doesInternationalPrefixExist(std::string(phoneNumber.substr(0, n)))) {
            truncNumber = phoneNumber.substr(n);
            std::string_view nationalNumberStub;
            int countryCodeStub = 0;

            // we check for country code to prevent false positives,
            // for example 011 69 355 555 is Serbian local number not international call
            if (tryParseAnyCountryCode(truncNumber, countryCodeStub, nationalNumberStub)) {
                return true;
            }
        }
    }
    return false;
}

bool PhoneNormalizer::tryParseStrictInternationalPrefix(std::string_view phoneNumber, std::string_view &truncNumber,
                                                        const std::string &isoCountry) const {
    if (isoCountry.empty()) {
        return false;
    }
    const NumberingPlan &plan = m_repo.getForIsoCountry(isoCountry);

    if (startsWith(phoneNumber, plan.internationalPrefix)) {
        truncNumber = phoneNumber.substr(plan.internationalPrefix.size());
        int countryCodeStub = 0;
        std::string_view nationalNumberStub;
        if (tryParseAnyCountryCode(truncNumber, countryCodeStub, nationalNumberStub)) {
            return true;
        }
    }
    return false;
}

// Normalizes phoneNumber to E.164 format ("+38160123456").
// we use origin country for local number parsing without country code and international prefix for international calls
// if strict true only origins country is used, if its false we check to find any that fits
PhoneNumber PhoneNormalizer::normalize(const std::string &phoneNumber, const std::string &orginCountryIso,
                                       bool strictOrigin) const {
    std::string clPhoneNumber = cleanPhoneNumber(phoneNumber);
    if (clPhoneNumber.empty()) {
        return {phoneNumber, 0, "", ""};
    }

    std::string_view phoneNumberView(clPhoneNumber);
    int countryCode = 0;
    std::string_view nationalNumber;

    if (phoneNumberView[0] == '+') {
        if (tryParseAnyCountryCode(phoneNumberView.substr(1), countryCode, nationalNumber)) {
            return {clPhoneNumber, countryCode, std::string(nationalNumber), clPhoneNumber};
        }

    } else { // try international prefix with country code, for example 0011 49 555 555 (australia calling germany)
            
        std::string_view truncNumber;
        if ((strictOrigin && tryParseStrictInternationalPrefix(phoneNumberView, truncNumber, orginCountryIso)) ||
            (!strictOrigin && tryParseAnyInternationalPrefix(phoneNumberView, truncNumber))) {

            tryParseAnyCountryCode(truncNumber, countryCode, nationalNumber);
            return {clPhoneNumber, countryCode, std::string(nationalNumber), '+' + std::string(truncNumber)};
        }
    }

    // no + or internationalPrefix, so we assume local format
    if (!orginCountryIso.empty()) {
        const NumberingPlan &plan = m_repo.getForIsoCountry(orginCountryIso);
        if (startsWith(phoneNumberView, plan.nationalPrefix)) {
            // Italy has a weird numbering plan where national prefix is not removed

            nationalNumber =
                orginCountryIso == "IT" ? phoneNumberView : phoneNumberView.substr(plan.nationalPrefix.size());
            countryCode = plan.countryCode;
            return {clPhoneNumber, countryCode, std::string(nationalNumber),
                    "+" + std::to_string(plan.countryCode) + std::string(nationalNumber)};
        }
    }

    return {clPhoneNumber, 0, "", ""};

} 

PhoneNormalizer::PhoneNormalizer(const std::string &metadata_path) { m_repo.loadPlans(metadata_path); }

PhoneNormalizer::PhoneNormalizer(const std::vector<NumberingPlan> &plans) : m_repo(plans) {}
