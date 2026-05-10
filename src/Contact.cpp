#include "Contact.hpp"

Contact::Contact(const std::string& name, const std::string& surname, std::list<PhoneNumber>& phoneNumbers, const std::string& email, Address* address)
    : name(name)
    , surname(surname)
    , phoneNumbers(phoneNumbers)
    , email(email)
    , address(address)
{}


std::string Contact::getName() const {
    return name;         
}

std::string Contact::getSurname() const {
    return surname;
}

std::list<PhoneNumber>& Contact::getPhoneNumbers() {
    return phoneNumbers;
}

std::string Contact::getEmail() const {
    return email;        
}

Address* Contact::getAddress() const { 
    return address;      
}

std::string Contact::getFullName() const {
    if (name.empty())    
        return surname;

    if (surname.empty()) 
        return name;
    
        return surname + ", " + name;
}

bool Contact::hasAddress() const {
    return address != nullptr && !address->isEmpty();
}

void Contact::addPhoneNumber(PhoneNumber phoneNumber) {
    for(auto &pn : phoneNumbers) {
        if (pn.getNormalizedValue() == phoneNumber.getNormalizedValue())
            return ;
    }
    phoneNumbers.push_back(phoneNumber);

    return ;
}