#ifndef CONTACT_HPP
#define CONTACT_HPP
#include "Address.hpp"
#include "PhoneNumber.hpp"

#include <string>
#include <list>

class Contact {
private:
    std::string m_name;
    std::string m_surname;
    std::list<PhoneNumber> m_phoneNumbers;
    std::string m_email;
    Address m_address;

public:
    Contact(const std::string& name, const std::string& surname, std::list<PhoneNumber> phoneNumbers, const std::string& email = "", Address address = Address());
   ~Contact() = default;

    std::string getName() const;
    std::string getSurname() const;
    std::list<PhoneNumber> getPhoneNumbers();
    std::string getEmail() const;
    Address getAddress() const;

    std::string getFullName() const;
    bool hasAddress() const;
    void addPhoneNumber(PhoneNumber PhoneNumber);
};
#endif