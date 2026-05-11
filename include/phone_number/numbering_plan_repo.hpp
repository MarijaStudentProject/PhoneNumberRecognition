#ifndef NUMBERING_PLAN_REPO_HPP
#define NUMBERING_PLAN_REPO_HPP

#include "numbering_plan.hpp"
#include <map>
#include <set>
#include <string>
#include <vector>

class NumberingPlanRepo {
  private:
    std::vector<NumberingPlan> m_plans;
    std::map<std::string, NumberingPlan *> m_isoPlanMap; // RS DE US, etc to plan
    std::map<int, NumberingPlan *> m_countryCodePlanMap; // 381, 49, etc to plan
    std::set<std::string> m_internationalPrefixSet;      // set of all prefixes for quick lookup

  public:
    NumberingPlanRepo() = default;
    ~NumberingPlanRepo() = default;

    NumberingPlanRepo(const NumberingPlanRepo &) = delete;
    NumberingPlanRepo &operator=(const NumberingPlanRepo &) = delete;
    NumberingPlanRepo(NumberingPlanRepo &&) = delete;
    NumberingPlanRepo &operator=(NumberingPlanRepo &&) = delete;

    void loadPlans(const std::string &filename);
    NumberingPlan getForIsoCountry(const std::string &isoCountry) const; // RS DE US, return empty if not found
    NumberingPlan getForCountryCode(int countryCode) const;              // 381, 49, return empty if not found
    bool doesCountryCodeExist(int countryCode) const;
    bool doesInternationalPrefixExist(const std::string &internationalPrefix) const;
};

#endif
