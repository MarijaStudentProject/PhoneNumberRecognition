#include "phone_number/phone_matcher.hpp"

bool PhoneMatcher::isMatch(const PhoneNumber &first, const PhoneNumber &second) const {
    if (first.getNormalizedValue().empty() || second.getNormalizedValue().empty()) {
        return false;
    }
    return first.getNormalizedValue() == second.getNormalizedValue();
}

std::optional<Contact> PhoneMatcher::findMatch(const PhoneNumber &phoneNumber,
                                               const std::vector<Contact> &contacts) const {
    for (const auto &contact : contacts) {
        for (const auto &contactPhone : contact.getPhoneNumbers()) {
            if (isMatch(phoneNumber, contactPhone)) {
                return contact;
            }
        }
    }
    return std::nullopt;
}
