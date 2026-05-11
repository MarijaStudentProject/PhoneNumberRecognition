#include "phone_number/contact.hpp"

Contact::Contact(const std::string &name, const std::string &surname, const std::vector<PhoneNumber> &phoneNumbers,
                 const std::string &email, const Address &address)
    : m_name(name), m_surname(surname), m_phoneNumbers(phoneNumbers), m_email(email), m_address(std::move(address)) {}

std::string Contact::getName() const { return m_name; }

std::string Contact::getSurname() const { return m_surname; }

const std::vector<PhoneNumber> &Contact::getPhoneNumbers() const { return m_phoneNumbers; }

bool Contact::hasPhoneNumbers() const { return !m_phoneNumbers.empty(); }

std::string Contact::getEmail() const { return m_email; }

const Address &Contact::getAddress() const { return m_address; }

PhoneNumber Contact::getPrimaryPhoneNumber() const {
    if (m_phoneNumbers.empty()) {
        return {};
    }
    return m_phoneNumbers[0];
}

std::string Contact::getFullName() const {
    if (m_name.empty()) {
        return m_surname;
    }

    if (m_surname.empty()) {
        return m_name;
    }

    return m_surname + ", " + m_name;
}

bool Contact::hasAddress() const { return !m_address.isEmpty(); }

void Contact::addPhoneNumber(const PhoneNumber &phoneNumber) {
    for (auto &pn : m_phoneNumbers) {
        if (pn.getNormalizedValue() == phoneNumber.getNormalizedValue()) {
            return;
        }
    }
    m_phoneNumbers.push_back(phoneNumber);

    return;
}
