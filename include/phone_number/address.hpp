#ifndef ADDRESS_HPP
#define ADDRESS_HPP
#include <string>

class Address {
  private:
    std::string m_street;
    std::string m_postalCode;
    std::string m_city;
    std::string m_country;

  public:
    Address(std::string street = "", std::string postalCode = "", std::string city = "", std::string country = "");
    
    std::string getStreet() const;
    std::string getPostalCode() const;
    std::string getCity() const;
    std::string getCountry() const;

    std::string getFormatted() const;
    bool isEmpty() const;

    bool operator==(const Address &other) const;
};
#endif
