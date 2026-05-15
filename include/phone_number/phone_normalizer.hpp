#ifndef PHONE_NORMALIZER_HPP
#define PHONE_NORMALIZER_HPP

#include "numbering_plan.hpp"
#include "numbering_plan_repo.hpp"
#include "phone_number.hpp"
#include <string>
#include <string_view>

class PhoneNormalizer {
  private:
    
    NumberingPlanRepo m_repo;

    bool tryParseAnyCountryCode(std::string_view phoneNumber, int &countryCode, std::string_view &nationalNumber) const;
    bool tryParseAnyInternationalPrefix(std::string_view phoneNumber, std::string_view &truncNumber) const;
    bool tryParseStrictInternationalPrefix(std::string_view phoneNumber, std::string_view &truncNumber, const std::string& isoCountry) const;

  public:
  
    PhoneNumber normalize(const std::string &phoneNumber,const std::string& orginCountryIso="RS", bool strict = false) const;
    PhoneNormalizer(const std::string &metadata_path);
};

#endif
