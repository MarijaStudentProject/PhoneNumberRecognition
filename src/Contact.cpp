#include "Contact.hpp"

Contact::Contact(const std::string& name, const std::string& surname, const std::string& email, Address* address)
    : name(name)
    , surname(surname)
    , email(email)
    , address(address)
{}


std::string Contact::getName() const {
    return name;         
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

