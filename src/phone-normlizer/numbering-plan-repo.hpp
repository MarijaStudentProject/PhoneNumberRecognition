#ifndef NUMBERING_PLAN_REPO_HPP
#define NUMBERING_PLAN_REPO_HPP

#include <string>
#include <vector>
#include "numbering-plan.hpp"
#include <map>
#include <set>

class NumberingPlanRepo {
private:
    std::vector<NumberingPlan> plans;
    std::map<std::string, NumberingPlan*> isoPlanMap;
    std::map<int, NumberingPlan*> countryCodePlanMap;
    std::set<std::string> internationalPrefixPlanSet;
public:
    NumberingPlanRepo();
    ~NumberingPlanRepo();

    void load(const std::string& filename); //TODO make metadata file
    NumberingPlan getForIsoCountry(const std::string& isoCountry) const;
    NumberingPlan getForCountryCode(int countryCode) const;
    bool doesCountryCodeExist(int countryCode) const;
    bool doesInternationalPrefixExist(std::string internationalPrefix) const;
 };

#endif