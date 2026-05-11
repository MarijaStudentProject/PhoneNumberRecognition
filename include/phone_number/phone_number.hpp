#ifndef PHONENUMBER_HPP
#define PHONENUMBER_HPP
#include <string>


class PhoneNumber {
    
    private:
    std::string m_rawValue;
    std::string m_normalizedValue;
    unsigned int m_countryCode = 0;
    std::string m_nationalNumber;
    
    public:
    PhoneNumber(const std::string& rawValue, unsigned int countryCode, const std::string& nationalNumber, const std::string& normalizedValue);
    PhoneNumber() = default;
    ~PhoneNumber() = default;

    std::string getRawValue() const;
    std::string getNormalizedValue() const;
    unsigned int getCountryCode() const;
    std::string getNationalNumber() const;


};

#endif