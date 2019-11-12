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
    IMPLY
};

std::ostream & operator<<(std::ostream & os, rule_operation const & rhs);


template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct rule;

using rule_node = std::variant<std::shared_ptr<fact>, std::shared_ptr<rule>>;

struct rule
{
    int name;
    rule_operation operation;
    rule_node left;
    rule_node right;
    std::shared_ptr<rule> parent;
    fact_value value;
    bool visited;

    template<class L, class R>
    rule(int name, rule_operation operation, L left, R right)
        : name{name}
        , operation{operation}
        , left(left)
        , right(right)
        , parent{}
        , value{fact_value::FALSE}
        , visited{false}
    {}

    fact_value * get_fact_value(rule_node node, int i);

    void evaluate(int i);
    void operation_not(fact_value * initial_value, fact_value * r_value, int i);
    void operation_and(fact_value * l_value, fact_value * r_value, int i);
    void operation_or(fact_value * l_value, fact_value * r_value, int i);
    void operation_xor(fact_value * l_value, fact_value * r_value, int i);
    void operation_imply(fact_value * l_value, fact_value * r_value, int i);
    void to_true(rule_node node);
};