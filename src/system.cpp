#include "system.hpp"

expert_system::expert_system()
    : facts{}
    , rules{}
{
    facts['A'] = fact{fact_value::TRUE};
    facts['B'] = fact{fact_value::TRUE};
    rules.push_back(rule{rule_operation::AND, &facts['A'], &facts['B']});
}

void expert_system::operator()()
{
    std::cout << rules[0].evaluate() << '\n';
}