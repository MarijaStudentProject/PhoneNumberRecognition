#include "phone_number/address.hpp"

Address::Address(const std::string &street, int postalCode, const std::string &city, const std::string &country)
    : m_street(street), m_postalCode(postalCode), m_city(city), m_country(country) {}

std::string Address::getStreet() const { return m_street; }

int Address::getPostalCode() const { return m_postalCode; }

std::string Address::getCity() const { return m_city; }

std::string Address::getCountry() const { return m_country; }

std::string Address::getFormatted() const {
    if (isEmpty()) {
        return "";
    }

    std::string result;

    if (!m_street.empty()) {
        result += m_street;
    }

    if (m_postalCode != 0 || !m_city.empty()) {
        if (!result.empty()) {
            result += ", ";
        }

        if (m_postalCode != 0) {
            result += std::to_string(m_postalCode) + " ";
        }

        result += m_city;
    }

    if (!m_country.empty()) {
        if (!result.empty()) {
            result += ", ";
        }

        result += m_country;
    }

    return result;
}

bool Address::isEmpty() const { return m_street.empty() && m_postalCode == 0 && m_city.empty() && m_country.empty(); }

bool Address::operator==(const Address &other) const {
    return m_street == other.m_street && m_postalCode == other.m_postalCode && m_city == other.m_city &&
           m_country == other.m_country;
}
