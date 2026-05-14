#include "phone_number/phone_matcher.hpp"
#include <algorithm>
#include <cctype>
#include <set>

std::map<std::string, Contact> PhoneMatcher::contactMap;

PhoneMatcher::PhoneMatcher(const PhoneNormalizer &normalizer)
    : m_normalizer(normalizer){}

bool PhoneMatcher::isMatch(const PhoneNumber &first, const PhoneNumber &second) const {
    if (first.getNormalizedValue().empty() || second.getNormalizedValue().empty()) {
        return false;
    }
    return first.getNormalizedValue() == second.getNormalizedValue();
}

std::vector<Contact> PhoneMatcher::findMatch(const std::string& phoneNumber,
                                               const std::vector<Contact> &contacts) const {
    PhoneNumber number = m_normalizer.normalize(phoneNumber, false);                                            
    for (const auto &contact : contacts) {
        if (!contact.hasPhoneNumbers()) {
            continue;
        }
        for (const auto &contactPhone : contact.getPhoneNumbers()) {
            if (isMatch(number, contactPhone)) {
                return std::vector<Contact>{contact};
            }
        }
    }
    // if we're here it means that the (normalized) match for a given number doesn't exist
    // we check for a naive substring partial match, we check if there is phone numbers that have the same last 5 digits as the given number
    std::string reversedNumberForSearch = number.getRawValue();
    std::reverse(reversedNumberForSearch.begin(), reversedNumberForSearch.end());
    auto results = findByPrefix(reversedNumberForSearch);
    return results;
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

std::vector<Contact> PhoneMatcher::findByPrefix(const std::string& number) {
    std::string prefix = number.substr(0, 5); // first 5 characters

    std::vector<Contact> results;
    std::set<Contact> uniqueContacts; // for no duplicates
    auto it = PhoneMatcher::contactMap.lower_bound(prefix);

    while (it != PhoneMatcher::contactMap.end() && it->first.substr(0, 5) == prefix) {
            if (uniqueContacts.find(it->second) == uniqueContacts.end()) {
            uniqueContacts.insert(it->second);
            results.push_back(it->second);
        }
        ++it;
    }

    return results;
}
