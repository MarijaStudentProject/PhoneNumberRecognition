#ifndef ADDRESS_HPP
#define ADDRESS_HPP
#include <string>

class Address {
private:
    std::string street;
    int postalCode = 0;
    std::string city;
    std::string country;

public:
    Address(const std::string& street, int postalCode, const std::string& city, const std::string& country);
    Address()  = default;
   ~Address()  = default;
    
    std::string getStreet() const;
    int getPostalCode() const;
    std::string getCity() const;
    std::string getCountry() const;

    std::string format() const;
    bool isEmpty() const;

    bool operator==(const Address& other) const;
};
#endif