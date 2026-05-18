#ifndef PHONE_MATCHER_HPP
#define PHONE_MATCHER_HPP

#include "phone_number/contact.hpp"
#include "phone_number/phone_number.hpp"

#include <map>
#include <optional>
#include <vector>
class PhoneMatcher {
  private:
    static int editDistance(const std::string &a, const std::string &b);
    static std::string toLower(std::string s);
    static bool endsWith(const std::string &str, const std::string_view &suffix);
    static bool startsWith(const std::string &str, const std::string_view &prefix);

  public:
    static bool isExactMatch(const PhoneNumber &first, const PhoneNumber &second);
    static std::optional<int> findMatch(const PhoneNumber &number, const std::vector<Contact> &contacts);
    static std::optional<int> matchBySuffix(std::string &number, const std::vector<Contact> &contacts, int minLen = 5,
                                            float threshold = 0.75F);
    static std::vector<int> searchByName(const std::string &name, const std::vector<Contact> &contacts, int threshold);
    static std::vector<int> searchByNumberPrefix(const std::string &number, const std::vector<Contact> &contacts);
};

#endif
