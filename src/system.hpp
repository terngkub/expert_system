#pragma once
#include "fact.hpp"
#include "rule.hpp"
#include <map>
#include <vector>

struct expert_system
{
    std::map<char, fact> facts;
    std::vector<rule> rules;

    expert_system();

    void operator()();
    fact_value query(fact * f);
};