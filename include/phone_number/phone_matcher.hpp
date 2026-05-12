#ifndef PHONE_MATCHER_HPP
#define PHONE_MATCHER_HPP

#include "phone_number/contact.hpp"
#include "phone_number/phone_number.hpp"

#include <optional>
#include <vector>

class PhoneMatcher {
  public:
    bool isMatch(const PhoneNumber &first, const PhoneNumber &second) const;
    std::optional<Contact> findMatch(const PhoneNumber &phoneNumber, const std::vector<Contact> &contacts) const;
};

#endif
