#ifndef ADDRESS_HPP
#define ADDRESS_HPP
#include <string>

class Address {
private:
    std::string street;
    int postalCode;
    std::string city;
    std::string country;

public:
    Address(const std::string& street = "", int postalCode = 0, const std::string& city = "", const std::string& country = "");
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
