#pragma once
#include "fact.hpp"
#include <iostream>
#include <variant>


// Rule Operation Enum
enum rule_operation
{
    NOT,
    AND,
    OR,
    XOR,
    IMPLY
};
std::ostream & operator<<(std::ostream & os, rule_operation const & rhs);


// Rule Node
struct rule;
using rule_node = std::variant<std::shared_ptr<fact>, std::shared_ptr<rule>>;


// Rule
class rule
{
public:
    // Public Attributes
    int name;
    rule_operation operation;
    rule_node left;
    rule_node right;
    std::shared_ptr<rule> parent;
    fact_value value;
    bool visited;

    // Constructor and Destructor
    rule() = delete;
    ~rule() = default;

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

    // Copy - Enable
    rule(rule const &) = default;
    rule & operator=(rule const &) = default;

    // Move - Enable
    rule(rule &&) = default;
    rule & operator=(rule &&) = default;

    // Public Methods
    void evaluate(int i);
    std::string get_name(rule_node node);

private:
    fact_value get_fact_value(rule_node node, int i);
    void operation_not(fact_value l_value, int i);
    void operation_and(fact_value l_value, fact_value r_value, int i);
    void operation_or(fact_value l_value, fact_value r_value, int i);
    void operation_xor(fact_value l_value, fact_value r_value, int i);
    void operation_imply(fact_value l_value, fact_value r_value, int i);
    void to_true(rule_node node, int i);
};