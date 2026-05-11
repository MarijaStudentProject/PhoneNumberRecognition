#include "phone_number/phone_normalizer.hpp"
#include <iostream>

namespace {
std::string cleanPhoneNumber(const std::string &phoneNumber) {
    std::string cleaned;
    for (char c : phoneNumber) {
        if ((isdigit(c) != 0) || c == '+' && cleaned.empty()) {
            cleaned += c;
        }
    }
    return cleaned;
}

void testNormalizePhoneNumber(const std::string &phoneNumber, PhoneNormalizer &normalizer, bool strict = true) {
    std::string cleaned = cleanPhoneNumber(phoneNumber);
    std::string normalizedNumber = normalizer.normalize(cleaned, strict);
    std::cout << "Original: " << phoneNumber << "\n";
    std::cout << "Cleaned: " << cleaned << "\n";
    std::cout << "Normalized: " << normalizedNumber << "\n";
    std::cout << "-----------------------------\n";
}
} // namespace

int main() {
    PhoneNormalizer normalizer{"../resources/min_filtered_metadata.json", "RS"};

    std::string phoneNumber = "066 555 555";
    testNormalizePhoneNumber(phoneNumber, normalizer);

    std::string phoneNumber2 = "+381 66 555 555";
    testNormalizePhoneNumber(phoneNumber2, normalizer);

    std::string phoneNumber3 = "00381 66 555 555";
    testNormalizePhoneNumber(phoneNumber3, normalizer);

    std::string phoneNumber4 = "011 66 555 555";
    testNormalizePhoneNumber(phoneNumber4, normalizer,
                             false); // +66 is Thailand, strict=false should match it as international

    std::string phoneNumber5 = "011 66 555 555";
    testNormalizePhoneNumber(phoneNumber5, normalizer, true); // strict matches local

    std::string phoneNumber6 = "011 693 555 555"; // strict matches local
    testNormalizePhoneNumber(phoneNumber6, normalizer, true);

    std::string phoneNumber7 = "011 693 555 555"; // +693 does not exist, should match local even in non strict mode
    testNormalizePhoneNumber(phoneNumber7, normalizer, false);

    std::string phoneNumber8 = "+1 (202) 555-0173";
    testNormalizePhoneNumber(phoneNumber8, normalizer);

    return 0;
}

/* expected output:
Original: 066 555 555
Cleaned: 066555555
Normalized: +38166555555
-----------------------------
Original: +381 66 555 555
Cleaned: +38166555555
Normalized: +38166555555
-----------------------------
Original: 00381 66 555 555
Cleaned: 0038166555555
Normalized: +38166555555
-----------------------------
Original: 011 66 555 555
Cleaned: 01166555555
Normalized: +66555555
-----------------------------
Original: 011 66 555 555
Cleaned: 01166555555
Normalized: +3811166555555
-----------------------------
Original: 011 693 555 555
Cleaned: 011693555555
Normalized: +38111693555555
-----------------------------
Original: 011 693 555 555
Cleaned: 011693555555
Normalized: +38111693555555
-----------------------------
Original: +1 (202) 555-0173
Cleaned: +12025550173
Normalized: +12025550173
*/