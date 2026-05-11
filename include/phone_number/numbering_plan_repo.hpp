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
    std::map<std::string, NumberingPlan *> m_isoPlanMap;
    std::map<int, NumberingPlan *> m_countryCodePlanMap;
    std::set<std::string> m_internationalPrefixSet;

  public:
    NumberingPlanRepo() = default;
    ~NumberingPlanRepo() = default;

    NumberingPlanRepo(const NumberingPlanRepo &) = delete;
    NumberingPlanRepo &operator=(const NumberingPlanRepo &) = delete;
    NumberingPlanRepo(NumberingPlanRepo &&) = delete;
    NumberingPlanRepo &operator=(NumberingPlanRepo &&) = delete;

    void load(const std::string &filename);
    NumberingPlan
    getForIsoCountry(const std::string &isoCountry) const;  // RS DE US, etc, if not found return empty plan
    NumberingPlan getForCountryCode(int countryCode) const; // 381, 49, return empty if not found
    bool doesCountryCodeExist(int countryCode) const;
    bool doesInternationalPrefixExist(const std::string &internationalPrefix) const;
};

#endif
