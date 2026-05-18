#include <algorithm>
#include <fstream>
#include <iostream>
#include <ivanenko/text_io.h>
#include <phone_number/phone_matcher.hpp>
#include <phone_number/vcf_parser.hpp>
#include <utility>

VcfParser::VcfParser(const PhoneNormalizer &normalizer, std::string parsingRegion)
    : m_normalizer(normalizer), m_parsingRegion(std::move(parsingRegion)) {}

std::vector<Contact> VcfParser::loadFromFile(const std::string &filePath) const {

    std::ifstream file;
    file.open(filePath);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << '\n';
        return {};
    }

    TextReader tr(file);

    std::vector<Contact> contacts;

    try {
        std::vector<vCard> cards = tr.parseCards();
        for (auto &card : cards) {
            contacts.push_back(cardToContact(card));
        }
    } catch (const std::exception &e) {
        std::cerr << "Error parsing vCard file: " << e.what() << '\n';
        return {};
    }

    return contacts;
}

Contact VcfParser::cardToContact(vCard &card) const {

    std::string name;
    std::string surname;
    std::vector<PhoneNumber> phoneNumbers;
    std::string email;
    Address address;

    std::vector<vCardProperty> properties = card.properties();

    for (auto &prop : properties) {
        std::string propName = prop.getName();
        std::vector<std::string> values = prop.values();
        if (values.empty()) {
            continue;
        }
        if (propName == VC_NAME) {
            name = values[vCardProperty::Firstname];
            surname = values[vCardProperty::Lastname];
        } else if (propName == VC_EMAIL) {
            email = values[0];
        } else if (propName == VC_TELEPHONE) {
            PhoneNumber phoneNumber = m_normalizer.normalize(values[0], m_parsingRegion);
            phoneNumbers.push_back(phoneNumber);
        } else if (propName == VC_ADDRESS) {

            std::string street = values.size() > vCardProperty::Street ? values[vCardProperty::Street] : "";
            std::string locality = values.size() > vCardProperty::Locality ? values[vCardProperty::Locality] : "";
            // std::string region = values.size() > vCardProperty::Region ? values[vCardProperty::Region] : "";
            std::string postalCode = values.size() > vCardProperty::PostalCode ? values[vCardProperty::PostalCode] : "";
            std::string country = values.size() > vCardProperty::Country ? values[vCardProperty::Country] : "";

            address = Address(street, postalCode, locality, country);
        }
    }
    return Contact(name, surname, phoneNumbers, email, address);
}
