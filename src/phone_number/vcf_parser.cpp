#include <phone_number/vcf_parser.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <ivanenko/text_io.h>
#include <phone_number/phone_matcher.hpp>

VcfParser::VcfParser(const PhoneNormalizer &normalizer)
    :m_normalizer(normalizer){}
std::vector<Contact> VcfParser::loadFromFile(const std::string &filePath) const {
    PhoneMatcher::contactMap.clear(); // in case we're loading the vcf file multiple times, we need to clean the previously stored records
    
    std::ifstream file;
    file.open(filePath);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }
    
    TextReader tr(file);

    std::vector<Contact> contacts;

    try {
        std::vector<vCard> cards = tr.parseCards();
        for (auto &card : cards) {
            Contact newContact = cardToContact(card);
            for(const auto &phoneNumber : newContact.getPhoneNumbers()) {
                std::string numberToReverse = phoneNumber.getRawValue();
                std::reverse(numberToReverse.begin(), numberToReverse.end());
                PhoneMatcher::contactMap.insert({numberToReverse, newContact});
            }
            contacts.push_back(newContact);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error parsing vCard file: " << e.what() << std::endl;
        return {};
    }

    return contacts;
}

Contact VcfParser::cardToContact(vCard &card) const {

    // Contact contact;
    std::string name;
    std::string surname;
    std::vector<PhoneNumber> phoneNumbers;
    std::string email;
    Address address;
   
    std::vector<vCardProperty> properties = card.properties();
            
    for (auto &prop : properties) {
        std::string propName = prop.getName();
        std::vector<std::string> values = prop.values();
       
        if (propName == VC_NAME && !values.empty()) {
            name = values[vCardProperty::Firstname];
            surname = values[vCardProperty::Lastname];
        } else if (propName == VC_EMAIL && !values.empty()) {
            email = values[0];
        } else if (propName == VC_TELEPHONE && !values.empty()) {
            PhoneNumber phoneNumber = m_normalizer.normalize(values[0]);
            phoneNumbers.push_back(phoneNumber);
        } else if (propName == VC_ADDRESS) {
            
            std::string street = values.size() > vCardProperty::Street ? values[vCardProperty::Street] : "";
            std::string locality = values.size() > vCardProperty::Locality ? values[vCardProperty::Locality] : "";
            std::string region = values.size() > vCardProperty::Region ? values[vCardProperty::Region] : "";
            std::string postalCode = values.size() > vCardProperty::PostalCode ? values[vCardProperty::PostalCode]
                                                                               : ""; 
            std::string country = values.size() > vCardProperty::Country ? values[vCardProperty::Country] : "";

            address = Address(street, postalCode, locality, country);
           
        }
    }
    return Contact(name, surname, phoneNumbers, email, address);
}
