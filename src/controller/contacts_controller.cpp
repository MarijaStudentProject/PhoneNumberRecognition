#include "controller/contacts_controller.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <iostream>
#include <phone_number/phone_matcher.hpp>

ContactsController::ContactsController()
    : m_normalizer(
          QDir(QCoreApplication::applicationDirPath()).filePath("resources/min_filtered_metadata.json").toStdString()),
      m_vcfParser(m_normalizer, "RS"), m_parsingRegion("RS") {}

ContactsController::ContactsController(const std::string &metadataPath, const std::string &parsingRegion)
    : m_normalizer(metadataPath), m_vcfParser(m_normalizer, parsingRegion), m_parsingRegion(parsingRegion) {}

const std::vector<Contact> &ContactsController::contacts() const { return m_contacts; }

std::vector<int> ContactsController::search(const std::string &text) const {
    auto result = PhoneMatcher::searchByName(text, m_contacts);
    auto result2 = PhoneMatcher::searchByNumberPrefix(text, m_contacts);
    result.insert(result.end(), result2.begin(), result2.end());
    return result;
}

Contact *ContactsController::findById(int id) {
    if (id < 0 || id >= m_contacts.size()) {
        return nullptr;
    }

    return &m_contacts[id];
}

void ContactsController::updateContact(int id, const std::string &name, const std::string surname,
                                       const std::vector<std::string> &phone, const std::string &email,
                                       const Address &address) {
    Contact *contact = findById(id);
    if (!contact) {
        return;
    }
    contact->setName(name);
    contact->setSurname(surname);
    contact->setEmail(email);
    contact->setAddress(address);
    std::vector<PhoneNumber> phoneNumbers;
    for (const auto &p : phone) {
        phoneNumbers.push_back(m_normalizer.normalize(p, m_parsingRegion, true));
    }
    contact->setPhoneNumbers(phoneNumbers);
}

int ContactsController::addContact(const std::string &name, const std::string &surname,
                                   const std::vector<std::string> &phones, const std::string &email,
                                   const Address &address) {
    std::vector<PhoneNumber> phoneNumbers;
    for (const auto &p : phones) {
        phoneNumbers.push_back(m_normalizer.normalize(p, m_parsingRegion, true));
    }
    m_contacts.push_back(Contact(name, surname, phoneNumbers, email, address));
    std::sort(m_contacts.begin(), m_contacts.end());
    for (int i = 0; i < static_cast<int>(m_contacts.size()); ++i) {
        if (m_contacts[i].getName() == name && m_contacts[i].getSurname() == surname)
            return i;
    }
    return static_cast<int>(m_contacts.size()) - 1;
}

void ContactsController::importContacts(const std::string &path) {
    m_contacts = m_vcfParser.loadFromFile(path);
    std::sort(m_contacts.begin(), m_contacts.end());
}

PhoneNumber ContactsController::normalize(const std::string &number, const std::string &country, bool strict) const {
    std::string isoCode = country.empty() ? m_parsingRegion : country;
    return m_normalizer.normalize(number, isoCode, strict);
}

std::optional<int> ContactsController::match(const std::string &number, const std::string &country) const {
    std::string isoCode = country.empty() ? m_parsingRegion : country;

    PhoneNumber phoneNumber = m_normalizer.normalize(number, isoCode, true);
    return PhoneMatcher::findMatch(phoneNumber, m_contacts);
}
