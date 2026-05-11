#ifndef PHONE_NORMALIZER_HPP
#define PHONE_NORMALIZER_HPP

#include "numbering_plan.hpp"
#include "numbering_plan_repo.hpp"
#include <string>

class PhoneNormalizer {
  private:
    // default region for phone parsing, if not specified otherwise
    // we will assume number belongs to this region to remove ambiguity
    // set to phone owner's country
    std::string m_localRegionIso;
    NumberingPlan m_localNumberPlan;

    NumberingPlanRepo m_repo;

    bool tryParseAnyCountryCode(std::string_view phoneNumber, int &countryCode, std::string_view &nationalNumber);
    bool tryParseAnyInternationalPrefix(std::string_view phoneNumber, std::string_view &truncNumber);

  public:
    std::string normalize(const std::string &phoneNumber, bool strict = false);
    PhoneNormalizer(const std::string &metadata_path, std::string localRegionIsoCountry);
    void setLocalRegionIso(const std::string &isoCountry);
};

#endif
