#include "phone_number/phone_normalizer.hpp"
#include <iostream>
#include <string_view>
#include <utility>

namespace {
bool startsWith(std::string_view str, std::string_view prefix) { return str.substr(0, prefix.size()) == prefix; }
} // namespace

// Tries to parse a country code from the start of phoneNumber (up to 3 digits).
// On success, sets countryCode and nationalNumber (the remainder after the code) and returns true.
bool PhoneNormalizer::tryParseAnyCountryCode(std::string_view phoneNumber, int &countryCode,
                                             std::string_view &nationalNumber) const {
    int currentCountryCode = 0;
    for (int n = 3; n > 0; n--) {
        if (phoneNumber.size() < n) {
            continue;
        }
        currentCountryCode = std::stoi(std::string(phoneNumber.substr(0, n)));

        if (m_repo.doesCountryCodeExist(currentCountryCode)) {
            countryCode = currentCountryCode;
            nationalNumber = phoneNumber.substr(n);
            return true;
        }
    }

    return false;
}

// Tries to match any known international dialling prefix at the start of phoneNumber.
// On success, sets truncNumber to the portion after the prefix (i.e. country code + national number)
// and returns true. A country-code check is performed to filter out false positives.
bool PhoneNormalizer::tryParseAnyInternationalPrefix(std::string_view phoneNumber,
                                                     std::string_view &truncNumber) const {
    for (int n = 4; n > 0; n--) { // maximal prefix is 4 digits
                                  // start from longest 0011 australia first then european 00
        if (m_repo.doesInternationalPrefixExist(std::string(phoneNumber.substr(0, n)))) {
            truncNumber = phoneNumber.substr(n);
            std::string_view nationalNumberStub;
            int countryCodeStub = 0;

            // we check for country code to prevent false positives,
            // for example 011 69 355 555 Serbian local number
            if (tryParseAnyCountryCode(truncNumber, countryCodeStub, nationalNumberStub)) {
                return true;
            }
        }
    }
    return false;
}

// check against local plans international prefix
bool PhoneNormalizer::tryParseStrictInternationalPrefix(std::string_view phoneNumber,
                                                        std::string_view &truncNumber) const {
    if (!m_localRegionIso.empty()) {
        return false;
    }
    if (startsWith(phoneNumber, m_localNumberPlan.m_internationalPrefix)) {
        truncNumber = phoneNumber.substr(m_localNumberPlan.m_internationalPrefix.size());
        int countryCodeStub = 0;
        std::string_view nationalNumberStub;
        if (tryParseAnyCountryCode(truncNumber, countryCodeStub, nationalNumberStub)) {
            return true;
        }
    }
    return false;
}

// Normalizes phoneNumber to E.164 format (e.g. "+38160123456").
// phoneNumber should be pre-cleaned (digits only, or a single leading '+' for already-international numbers).
// default region is used for local number parsing without country code.
// strict=true uses only the local region's international prefix (recommended for local contacts);
// use strict=false for incoming calls where any known international prefix is accepted.
std::string PhoneNormalizer::normalize(const std::string &phoneNumber, bool strict) const {
    if (phoneNumber.empty()) {
        return phoneNumber;
    }

    std::string_view phoneNumberView(phoneNumber);
    int countryCode = 0;
    std::string normalizedNumber;
    (void)normalizedNumber; // we will return this later
    std::string_view nationalNumber;

    if (phoneNumberView[0] == '+') { // already in international format no normalization needed
        if (tryParseAnyCountryCode(phoneNumberView.substr(1), countryCode, nationalNumber)) {
            return phoneNumber;
        }

    } else { // if not, try international prefix with country code, for example 00381 66 555 555,
             // 0011 49 555 555 (australia calling germany)
        std::string_view truncNumber;
        if ((strict && tryParseStrictInternationalPrefix(phoneNumberView, truncNumber)) ||
            (!strict && tryParseAnyInternationalPrefix(phoneNumberView, truncNumber))) {
            tryParseAnyCountryCode(truncNumber, countryCode, nationalNumber);
            return '+' + std::string(truncNumber);
        }
    }

    // no + or internationalPrefix, so we assume local format
    if (!m_localRegionIso.empty()) {
        if (startsWith(phoneNumberView, m_localNumberPlan.m_nationalPrefix)) {
            // Italy has a weird numbering plan where national prefix is not removed
            // for example 066 555 555 -> +39 066 555 555
            nationalNumber = m_localRegionIso == "IT"
                                 ? phoneNumberView
                                 : phoneNumberView.substr(m_localNumberPlan.m_nationalPrefix.size());
            return "+" + std::to_string(m_localNumberPlan.m_countryCode) + std::string(nationalNumber);
        }
    }

    return phoneNumber; // we dont know how to parse it, return as is

} // TODO consider adding length check

// Constructs a PhoneNormalizer that loads numbering-plan metadata from metadata_path
// and sets the default local region to localRegionIsoCountry (e.g. "RS", "DE").
PhoneNormalizer::PhoneNormalizer(const std::string &metadata_path, std::string localRegionIsoCountry)
    : m_localRegionIso(std::move(localRegionIsoCountry)) {
    m_repo.loadPlans(metadata_path);
    m_localNumberPlan = m_repo.getForIsoCountry(m_localRegionIso);
}

// Updates the default local region used for parsing numbers without a country code or international prefix.
void PhoneNormalizer::setLocalRegionIso(const std::string &isoCountry) {
    m_localRegionIso = isoCountry;
    m_localNumberPlan = m_repo.getForIsoCountry(m_localRegionIso);
}
