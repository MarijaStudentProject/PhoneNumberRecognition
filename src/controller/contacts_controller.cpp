#include "controller/contacts_controller.h"

#include <phone_number/phone_matcher.hpp>

ContactsController::ContactsController()
    : m_normalizer("resources/min_filtered_metadata.json"), m_vcfParser(m_normalizer, "RS"){
}

const std::vector<Contact> &ContactsController::contacts() const{
    return m_contacts;
}

std::vector<int> ContactsController::searchByName(const std::string &text) const{
    return PhoneMatcher::searchByName(text,m_contacts);
}

Contact *ContactsController::findById(int id){
    if(id < 0 || id >= m_contacts.size()){
        return nullptr;
    }

    return &m_contacts[id];
}

void ContactsController::updateContact(int id, const std::string &name, const std::string surname, const std::string &phone, const std::string &email, const std::string &address){
    Contact *contact = findById(id);
    //TODO
    //Add setters in Contacts if nessesery
}

void ContactsController::importContacts(const std::string& path){
   m_contacts =  m_vcfParser.loadFromFile(path);
}

std::optional<int> ContactsController::match(const std::string& number){
    PhoneNumber phoneNumber =  m_normalizer.normalize(number,"RS", true);
    return PhoneMatcher::findMatch(phoneNumber, m_contacts);
}

