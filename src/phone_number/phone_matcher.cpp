#include "phone_number/phone_matcher.hpp"
#include <algorithm>
#include <cctype>
#include <set>
#include <codecvt>
#include <locale>

bool PhoneMatcher::isMatch(const PhoneNumber &first, const PhoneNumber &second) {
    if (first.getNormalizedValue().empty() || second.getNormalizedValue().empty()) {
        return false;
    }
    return first.getNormalizedValue() == second.getNormalizedValue();
}

std::vector<Contact> PhoneMatcher::findMatch(const PhoneNumber &number,
                                               const std::vector<Contact> &contacts, const std::map<std::string, Contact> &contactMap) {
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
    std::string numberRaw = number.getRawValue();
    auto results = findBySuffix(numberRaw, contactMap);

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

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv; // conversion for unicode characters
    std::u32string wa = conv.from_bytes(a);
    std::u32string wb = conv.from_bytes(b);

    int m = wa.size(), n = wb.size();
    std::vector<int> prev(n+1), curr(n+1);

    for (int j = 0; j <= n; j++) prev[j] = j;

    for (int i = 1; i <= m; i++) {
        curr[0] = i;
        for (int j = 1; j <= n; j++) {
            if (wa[i-1] == wb[j-1])
                curr[j] = prev[j-1];
            else
                curr[j] = 1 + std::min({prev[j], curr[j-1], prev[j-1]});
        }
        swap(prev, curr);
    }
    return prev[n];
}

std::string PhoneMatcher::toLower(std::string s) {
    // std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return s;
}
// the function accepts a number, reverses it and then searches thru a map of reversed numbers for a possible prefix match
std::vector<Contact> PhoneMatcher::findBySuffix(std::string& number, const std::map<std::string, Contact> &contactMap) {

    std::reverse(number.begin(), number.end()); // reverses the number

    std::string prefix = number.substr(0, 5); // first 5 characters

    std::vector<Contact> results;
    std::set<Contact> uniqueContacts; // for no duplicates
    auto it = contactMap.lower_bound(prefix);

    while (it != contactMap.end() && it->first.substr(0, 5) == prefix) {
            if (uniqueContacts.find(it->second) == uniqueContacts.end()) {
            uniqueContacts.insert(it->second);
            results.push_back(it->second);
        }
        ++it;
    }

    return results;
}
