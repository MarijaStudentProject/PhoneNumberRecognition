#ifndef CONTACT_HPP
#define CONTACT_HPP
#include "address.hpp"
#include "phone_number.hpp"

#include <string>
#include <vector>

class Contact {
  private:
    std::string m_name;
    std::string m_surname;
    std::vector<PhoneNumber> m_phoneNumbers;
    std::string m_email;
    Address m_address;

  public:
    Contact(const std::string &name, const std::string &surname, const std::vector<PhoneNumber> &phoneNumbers,
            const std::string &email = "", const Address &address = {});
    ~Contact() = default;

    std::string getName() const;
    std::string getSurname() const;
    const std::vector<PhoneNumber> &getPhoneNumbers() const;
    bool hasPhoneNumbers() const;
    PhoneNumber getPrimaryPhoneNumber() const;
    std::string getEmail() const;
    const Address &getAddress() const;

    std::string getFullName() const;
    bool hasAddress() const;
    void addPhoneNumber(const PhoneNumber &phoneNumber);
};
#endif