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
    std::vector<rule *> rules;

    fact();
    fact(fact_value value);
};