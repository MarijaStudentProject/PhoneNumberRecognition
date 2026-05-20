#include "controller/contacts_controller.hpp"

#include <phone_number/phone_matcher.hpp>
#include <iostream>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

ContactsController::ContactsController()
    : m_normalizer(QDir(QCoreApplication::applicationDirPath())
                       .filePath("resources/min_filtered_metadata.json").toStdString()),
      m_vcfParser(m_normalizer, "RS"){
}

const std::vector<Contact> &ContactsController::contacts() const{
    return m_contacts;
}

std::vector<int> ContactsController::search(const std::string &text) const{
     auto result = PhoneMatcher::searchByName(text,m_contacts);
     auto result2 = PhoneMatcher::searchByNumberPrefix(text, m_contacts);
     result.insert(result.end(), result2.begin(), result2.end());
     return result;
}

Contact *ContactsController::findById(int id){
    if(id < 0 || id >= m_contacts.size()){
        return nullptr;
    }

    return &m_contacts[id];
}

void ContactsController::updateContact(int id, const std::string &name, const std::string surname, const std::vector<std::string> &phone, const std::string &email, const Address &address){
    Contact *contact = findById(id);
    if(!contact){
        return;
    }
    contact->setName(name);
    contact->setSurname(surname);
    contact->setEmail(email);
    contact->setAddress(address);
    std::vector<PhoneNumber> phoneNumbers;
    for(const auto& p : phone){
        phoneNumbers.push_back(m_normalizer.normalize(p, "RS", true));
    }
    contact->setPhoneNumbers(phoneNumbers);
}

void ContactsController::importContacts(const std::string& path){
    m_contacts =  m_vcfParser.loadFromFile(path);
    std::sort(m_contacts.begin(), m_contacts.end());

}

std::optional<int> ContactsController::match(const std::string& number){
    PhoneNumber phoneNumber =  m_normalizer.normalize(number,"RS", true);
    return PhoneMatcher::findMatch(phoneNumber, m_contacts);
}

