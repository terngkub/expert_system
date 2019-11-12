#pragma once
#include <iostream>
#include <vector>
#include <memory>

enum class fact_value
{
    TRUE,
    FALSE
};

std::ostream & operator<<(std::ostream & os, fact_value const & rhs);

struct rule;

struct fact
{
    char name;
    fact_value value;
    std::vector<std::shared_ptr<rule>> rules;
    bool visited;

    fact();
    fact(char name);
};