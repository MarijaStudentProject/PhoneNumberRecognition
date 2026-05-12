#ifndef VCF_PARSER_HPP
#define VCF_PARSER_HPP

#include "contact.hpp"
#include "phone_normalizer.hpp"
#include <istream>
#include <ivanenko/vcard.h>
#include <string>
#include <vector>

class VcfParser {
  public:
    VcfParser(const PhoneNormalizer &normalizer);

    std::vector<Contact> loadFromFile(const std::string &filePath) const;

  private:
    const PhoneNormalizer &m_normalizer;

    Contact cardToContact(vCard &card) const;
};

#endif // VCF_PARSER_HPP