#include "phone_number/phone_matcher.hpp"
#include <vector>
#include <algorithm>
#include <cctype>

bool PhoneMatcher::isMatch(const PhoneNumber &first, const PhoneNumber &second) const {
    if (first.getNormalizedValue().empty() || second.getNormalizedValue().empty()) {
        return false;
    }
    return first.getNormalizedValue() == second.getNormalizedValue();
}

std::optional<Contact> PhoneMatcher::findMatch(const PhoneNumber &phoneNumber,
                                               const std::vector<Contact> &contacts) const {
    for (const auto &contact : contacts) {
        if (!contact.hasPhoneNumbers()) {
            continue;
        }
        for (const auto &contactPhone : contact.getPhoneNumbers()) {
            if (isMatch(phoneNumber, contactPhone)) {
                return contact;
            }
        }
    }
    return std::nullopt;
}

std::vector<Contact> PhoneMatcher::findMatchByName(const std::string &name, const std::vector<Contact> &contacts) {
    std::vector<Contact> possibleMatches;
    std::string lowerName = toLower(name);
    for(const auto &contact : contacts) {
        if(editDistance(lowerName, toLower(contact.getFullName())) <= 2) {
            possibleMatches.push_back(contact);
        } else if(editDistance(lowerName, toLower(contact.getName())) <= 2) {
            possibleMatches.push_back(contact);
        } else if(editDistance(lowerName, toLower(contact.getSurname())) <= 2) {
            possibleMatches.push_back(contact);
        }
    
    }
    return possibleMatches;
}

int PhoneMatcher::editDistance(const std::string& a, const std::string& b) {
    int m = a.size(), n = b.size();
    std::vector<int> prev(n+1), curr(n+1);

    for (int j = 0; j <= n; j++) prev[j] = j;

    for (int i = 1; i <= m; i++) {
        curr[0] = i;
        for (int j = 1; j <= n; j++) {
            if (a[i-1] == b[j-1])
                curr[j] = prev[j-1];
            else
                curr[j] = 1 + std::min({prev[j], curr[j-1], prev[j-1]});
        }
        swap(prev, curr);
    }
    return prev[n];
}

std::string PhoneMatcher::toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}
