#pragma once
#include "fact.hpp"
#include <iostream>
#include <variant>

enum rule_operation
{
    NOT,
    AND,
    OR,
    XOR,
    IMPLY,
    IAOF
};

std::ostream & operator<<(std::ostream & os, rule_operation const & rhs);


template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct rule;

using rule_node = std::variant<fact *, rule *>;

struct rule
{
    rule_operation operation;
    rule_node left;
    rule_node right;

    template<class L, class R>
    rule(rule_operation operation, L left, R right)
        : operation{operation}
        , left(left)
        , right(right)
    {}

    fact_value get_fact_value(rule_node node);
    fact_value evaluate();
};