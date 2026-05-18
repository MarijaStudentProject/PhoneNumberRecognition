#include "phone_number/phone_number.hpp"
#include <utility>

PhoneNumber::PhoneNumber(std::string m_rawValue, int m_countryCode, std::string m_nationalNumber,
                         std::string m_normalizedValue)
    : m_rawValue(std::move(m_rawValue)), m_normalizedValue(std::move(m_normalizedValue)), m_countryCode(m_countryCode),
      m_nationalNumber(std::move(m_nationalNumber)) {}

std::string PhoneNumber::getRawValue() const { return m_rawValue; }

int PhoneNumber::getCountryCode() const { return m_countryCode; }
std::string PhoneNumber::getNormalizedValue() const { return m_normalizedValue; }

std::string PhoneNumber::getNationalNumber() const { return m_nationalNumber; }

bool PhoneNumber::operator==(const PhoneNumber &other) const {
    if (this == &other) {
        return true;
    }
    if (this->m_rawValue == other.m_rawValue) {
        return true;
    }
    if (isRaw() != other.isRaw()) {
        return false;
    }
    if (isRaw() && other.isRaw()) {
        return m_rawValue == other.m_rawValue;
    }

    return m_normalizedValue == other.m_normalizedValue;
}
