#ifndef ADDRESS_HPP
#define ADDRESS_HPP
#include <string>

class Address {
  private:
    std::string m_street;
    int m_postalCode;
    std::string m_city;
    std::string m_country;

  public:
    Address(const std::string &street = "", int postalCode = 0, const std::string &city = "",
            const std::string &country = "");
    ~Address() = default;

    std::string getStreet() const;
    int getPostalCode() const;
    std::string getCity() const;
    std::string getCountry() const;

    std::string getFormatted() const;
    bool isEmpty() const;

    bool operator==(const Address &other) const;
};
#endif
