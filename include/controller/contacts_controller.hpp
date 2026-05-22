#ifndef CONTACTS_CONTROLLER_H
#define CONTACTS_CONTROLLER_H

#include "phone_number/contact.hpp"
#include "phone_number/phone_normalizer.hpp"
#include "phone_number/vcf_parser.hpp"
#include <optional>
#include <string>
#include <vector>

class ContactsController {

  public:
    ContactsController();

    ContactsController(const std::string &metadataPath, const std::string &parsingRegion);

    const std::vector<Contact> &contacts() const;
    std::vector<int> search(const std::string &text) const;
    Contact *findById(int id);

    void updateContact(int id, const std::string &name, const std::string surname,
                       const std::vector<std::string> &phone, const std::string &email, const Address &address);
    int addContact(const std::string &name, const std::string &surname, const std::vector<std::string> &phones,
                   const std::string &email, const Address &address);

    void importContacts(const std::string &path);
    std::optional<int> match(const std::string &number, const std::string &country) const;

  private:
    std::vector<Contact> m_contacts;
    PhoneNormalizer m_normalizer;
    VcfParser m_vcfParser;
    std::string m_parsingRegion;
};

#endif // CONTACTS_CONTROLLER_H
