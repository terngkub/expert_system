#pragma once
#include <iostream>
#include <vector>

enum class fact_value
{
    TRUE,
    FALSE,
    UNDETERMINED
};

std::ostream & operator<<(std::ostream & os, fact_value const & rhs);

struct rule;

struct fact
{
    fact_value value;
    std::vector<std::shared_ptr<rule>> rules;

    fact();
    fact(fact_value value);
    fact(fact_value value, std::shared_ptr<rule> r);

    bool visited;
};