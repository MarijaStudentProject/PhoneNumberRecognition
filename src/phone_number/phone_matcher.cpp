#include "phone_number/phone_matcher.hpp"
#include <algorithm>
#include <cctype>
#include <codecvt>
#include <locale>
#include <optional>
#include <set>

bool PhoneMatcher::isExactMatch(const PhoneNumber &first, const PhoneNumber &second) {
    if (first.getNormalizedValue().empty() || second.getNormalizedValue().empty()) {
        return false;
    }
    return first.getNormalizedValue() == second.getNormalizedValue();
}

// tries to find an exact match for a given number, or tries a partial match by suffix and returns index
std::optional<int> PhoneMatcher::findMatch(const PhoneNumber &number, const std::vector<Contact> &contacts) {
    for (int i = 0; i < contacts.size(); i++) {
        const auto &contact = contacts[i];
        if (!contact.hasPhoneNumbers()) {
            continue;
        }
        for (const auto &contactPhone : contact.getPhoneNumbers()) {
            if (isExactMatch(number, contactPhone)) {
                return {i};
            }
        }
    }

    // try number raw sufix match
    std::string numberRaw = number.getRawValue();
    return PhoneMatcher::matchBySuffix(numberRaw, contacts);
}

std::vector<int> PhoneMatcher::searchByName(const std::string &name, const std::vector<Contact> &contacts,
                                            int threshold) {
    if (name.empty()) {
        return {};
    }
    std::vector<int> possibleMatches;
    std::string lowerName = toLower(name);
    for (int i = 0; i < contacts.size(); i++) {
        const auto &contact = contacts[i];
        if (toLower(contact.getFullName()) == lowerName || toLower(contact.getName()) == lowerName ||
            toLower(contact.getSurname()) == lowerName) {
            possibleMatches.push_back(i);
        } else if (startsWith(toLower(contact.getFullName()), lowerName) ||
                   startsWith(toLower(contact.getName()), lowerName) ||
                   startsWith(toLower(contact.getSurname()), lowerName)) {
            possibleMatches.push_back(i);
        } else if (name.length() >= 4) {
            if (editDistance(lowerName, toLower(contact.getFullName())) <= threshold ||
                editDistance(lowerName, toLower(contact.getName())) <= threshold ||
                editDistance(lowerName, toLower(contact.getSurname())) <= threshold) {
                possibleMatches.push_back(i);
            }
        }
    }
    return possibleMatches;
}

int PhoneMatcher::editDistance(const std::string &a, const std::string &b) {

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv; // conversion for unicode characters
    std::u32string wa = conv.from_bytes(a);
    std::u32string wb = conv.from_bytes(b);

    int m = static_cast<int>(wa.size());
    int n = static_cast<int>(wb.size());
    std::vector<int> prev(n + 1);
    std::vector<int> curr(n + 1);

    for (int j = 0; j <= n; j++) {
        prev[j] = j;
    }

    for (int i = 1; i <= m; i++) {
        curr[0] = i;
        for (int j = 1; j <= n; j++) {
            if (wa[i - 1] == wb[j - 1]) {
                curr[j] = prev[j - 1];
            } else {
                curr[j] = 1 + std::min({prev[j], curr[j - 1], prev[j - 1]});
            }
        }
        swap(prev, curr);
    }
    return prev[n];
}

std::string PhoneMatcher::toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

bool PhoneMatcher::endsWith(const std::string &str, const std::string_view &suffix) {
    if (str.size() < suffix.size()) {
        return false;
    }
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool PhoneMatcher::startsWith(const std::string &str, const std::string_view &prefix) {
    if (str.size() < prefix.size()) {
        return false;
    }
    return str.compare(0, prefix.size(), prefix) == 0;
}

std::optional<int> PhoneMatcher::matchBySuffix(std::string &number, const std::vector<Contact> &contacts, int minLen,
                                               float threshold) {
    // for phones to match they need to have suffix of at least minLen, and at least threshold% of both numbers size
    int n = static_cast<int>(number.size());
    std::string_view numberView(number);

    for (int len = n; len >= minLen; len--) {
        std::string_view suffix = numberView.substr(n - len);
        for (int i = 0; i < contacts.size(); i++) {
            const auto &contact = contacts[i];
            for (const auto &phone : contact.getPhoneNumbers()) {
                if (endsWith(phone.getRawValue(), suffix)) {
                    if ((float)suffix.size() / phone.getRawValue().size() >= threshold &&
                        (float)suffix.size() / static_cast<float>(number.size()) >= threshold) {
                        return {i};
                    }
                }
            }
        }
    }
    return std::nullopt;
}

std::vector<int> PhoneMatcher::searchByNumberPrefix(const std::string &number, const std::vector<Contact> &contacts) {
    std::vector<int> matches;
    if (number.empty()) {
        return matches;
    }
    for (int i = 0; i < contacts.size(); i++) {
        const auto &contact = contacts[i];
        if (!contact.hasPhoneNumbers()) {
            continue;
        }
        for (const auto &contactPhone : contact.getPhoneNumbers()) {
            if (PhoneMatcher::startsWith(contactPhone.getRawValue(), number) ||
                PhoneMatcher::startsWith(contactPhone.getNormalizedValue(), number)) {
                matches.push_back(i);
                break;
            }
        }
    }
    return matches;
}