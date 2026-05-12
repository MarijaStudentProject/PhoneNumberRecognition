#ifndef VCF_PARSER_HPP
#define VCF_PARSER_HPP

#include "phone_normalizer.hpp"
#include "contact.hpp"
#include <vector>
#include <string>
#include <istream>
#include <ivanenko/vcard.h>


class VcfParser {
  public:
    VcfParser(const PhoneNormalizer &normalizer);

    std::vector<Contact> loadFromFile(const std::string &filePath) const;

  private:
    const PhoneNormalizer &m_normalizer;

    Contact    cardToContact( vCard &card) const;
};


#endif // VCF_PARSER_HPP