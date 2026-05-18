#ifndef PHONENUMBER_HPP
#define PHONENUMBER_HPP
#include <string>

class PhoneNumber {

  private:
    std::string m_rawValue;
    std::string m_normalizedValue;
    int m_countryCode = 0;
    std::string m_nationalNumber;

  public:
    PhoneNumber(std::string rawValue, int countryCode, std::string nationalNumber, std::string normalizedValue);
    PhoneNumber() = default;

    std::string getRawValue() const;
    std::string getNormalizedValue() const;
    int getCountryCode() const;
    std::string getNationalNumber() const;
    bool isEmpty() const { return m_rawValue.empty(); }
    bool isRaw() const { return m_normalizedValue.empty(); }
    bool operator==(const PhoneNumber &other) const;
};

#endif