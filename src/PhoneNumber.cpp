#include "PhoneNumber.hpp"

// NOLINT(bugprone-easily-swappable-parameters)

PhoneNumber::PhoneNumber(const std::string& m_rawValue, unsigned int m_countryCode, const std::string& m_nationalNumber, const std::string& m_normalizedValue)
    : m_rawValue(m_rawValue), m_normalizedValue(m_normalizedValue), m_countryCode(m_countryCode), m_nationalNumber(m_nationalNumber) {}

std::string PhoneNumber::getRawValue() const {
    return m_rawValue;
}

unsigned int PhoneNumber::getCountryCode() const{
    return m_countryCode;
}
std::string PhoneNumber::getNormalizedValue() const {
    return m_normalizedValue;
 }

std::string PhoneNumber::getNationalNumber() const {
    return m_nationalNumber;
}

