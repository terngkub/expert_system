#pragma once
#include "fact.hpp"
#include "rule.hpp"
#include <map>
#include <vector>
#include <memory>

struct expert_system
{
    std::map<char, std::shared_ptr<fact>> facts;
    std::vector<std::shared_ptr<rule>> rules;

    expert_system();

    void operator()();
    fact_value query(std::shared_ptr<fact> f);
};