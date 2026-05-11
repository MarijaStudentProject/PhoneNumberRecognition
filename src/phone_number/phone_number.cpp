#include "phone_number/phone_number.hpp"

#include <utility>

PhoneNumber::PhoneNumber(std::string m_rawValue, unsigned int m_countryCode, std::string m_nationalNumber,
                         std::string m_normalizedValue)
    : m_rawValue(std::move(m_rawValue)), m_normalizedValue(std::move(m_normalizedValue)), m_countryCode(m_countryCode),
      m_nationalNumber(std::move(m_nationalNumber)) {}

std::string PhoneNumber::getRawValue() const { return m_rawValue; }

unsigned int PhoneNumber::getCountryCode() const { return m_countryCode; }
std::string PhoneNumber::getNormalizedValue() const { return m_normalizedValue; }

std::string PhoneNumber::getNationalNumber() const { return m_nationalNumber; }
