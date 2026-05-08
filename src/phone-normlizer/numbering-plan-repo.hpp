#ifndef NUMBERING_PLAN_REPO_HPP
#define NUMBERING_PLAN_REPO_HPP

#include <string>
#include <vector>
#include "numbering-plan.hpp"
#include <map>

class NumberingPlanRepo {
private:
    std::vector<NumberingPlan> plans;
    std::map<std::string, NumberingPlan*> isoPlanMap;
    std::map<int, NumberingPlan*> countryCodePlanMap;

public:
    NumberingPlanRepo();
    ~NumberingPlanRepo();

    void load(const std::string& filename); //TODO make metadata file
    NumberingPlan getForIsoCountry(const std::string& isoCountry) const;
    bool doesCountryCodeExist(int countryCode) const;
    NumberingPlan getForCountryCode(int countryCode) const;
 };

#endif