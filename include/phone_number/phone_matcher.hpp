#ifndef PHONE_MATCHER_HPP
#define PHONE_MATCHER_HPP

#include "phone_number/contact.hpp"
#include "phone_number/phone_number.hpp"

#include <vector>
#include <map>
class PhoneMatcher {
  private:
    // const PhoneNormalizer &m_normalizer;
    static int editDistance(const std::string& a, const std::string& b);
    static std::string toLower(std::string s);
    static std::vector<Contact> findBySuffix(std::string& number, const std::map<std::string, Contact> &contactMap);
  public:
    // PhoneMatcher(const PhoneNormalizer &normalizer);
    // static std::map<std::string, Contact> contactMap;
    static bool isMatch(const PhoneNumber &first, const PhoneNumber &second);
    static std::vector<Contact> findMatch(const PhoneNumber &number, const std::vector<Contact>& contacts, const std::map<std::string, Contact> &contactMap);
    static std::vector<Contact> findMatchByName(const std::string &name, const std::vector<Contact> &contacts);
};

#endif
