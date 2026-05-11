#include "Address.hpp"

Address::Address(const std::string& street, int postalCode, const std::string& city, const std::string& country)
    : street(street)
    , postalCode(postalCode)
    , city(city)
    , country(country)
{}

std::string Address::getStreet() const { 
    return street;     
}

int Address::getPostalCode() const {
    return postalCode; 
}

std::string Address::getCity() const {
    return city;       
}

std::string Address::getCountry() const {
    return country;    
}

std::string Address::format() const {
    if (isEmpty()) {
        return "";
    }

    std::string result;

    if (!street.empty()) {
        result += street;
    }

    if (postalCode != 0 || !city.empty()) {
        if (!result.empty()) {
            result += ", ";
        }

        if (postalCode != 0) {
            result += std::to_string(postalCode) + " ";
        }

        result += city;
    }

    if (!country.empty()) {
        if (!result.empty()) {
            result += ", ";
        }

        result += country;
    }

    return result;
}

bool Address::isEmpty() const {
    return street.empty() && postalCode == 0 && city.empty() && country.empty();
}

bool Address::operator==(const Address& other) const {
    return street == other.street && postalCode == other.postalCode && city == other.city && country == other.country;
}
