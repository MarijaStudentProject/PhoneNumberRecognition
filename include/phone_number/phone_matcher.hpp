#ifndef PHONE_MATCHER_HPP
#define PHONE_MATCHER_HPP

#include "phone_number/contact.hpp"
#include "phone_number/phone_number.hpp"
#include "phone_number/phone_normalizer.hpp"

#include <vector>
#include <map>
class PhoneMatcher {
  private:
    const PhoneNormalizer &m_normalizer;
    static int editDistance(const std::string& a, const std::string& b);
    static std::string toLower(std::string s);
    static std::vector<Contact> findByPrefix(const std::string& number);
  public:
    static std::map<std::string, Contact> contactMap;
    PhoneMatcher(const PhoneNormalizer &normalizer);
    bool isMatch(const PhoneNumber &first, const PhoneNumber &second) const;
    std::vector<Contact> findMatch(const std::string& phoneNumber, const std::vector<Contact>& contacts) const;
    static std::vector<Contact> findMatchByName(const std::string &name, const std::vector<Contact> &contacts);
};

#endif
