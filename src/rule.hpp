#pragma once
#include "fact.hpp"
#include <iostream>
#include <memory>
#include <variant>
#include <vector>


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
    std::string get_name(rule_node node);
    void evaluate(int i);

private:
    // Public Methods
    fact_value get_fact_value(rule_node node, int i);

    // Operations
    void operation_not(fact_value l_value, int i);
    void operation_and(fact_value l_value, fact_value r_value, int i);
    void operation_or(fact_value l_value, fact_value r_value, int i);
    void operation_xor(fact_value l_value, fact_value r_value, int i);
    void operation_imply(fact_value l_value, fact_value r_value, int i);
    void to_true(rule_node node, int i);
    void to_true_parent(int i);
    fact_value to_true_get_fact_value(rule_node const & node, int i);

    // Visualisation
    void pv_getfactvalue_begin(int i, std::shared_ptr<fact> const & f);
    void pv_getfactvalue_evaluate_begin(int i, std::vector<std::shared_ptr<rule>> const & rules);
    void pv_getfactvalue_evaluate_end(int i, std::vector<std::shared_ptr<rule>> const & rules);
    void pv_getfactvalue_end(int i, std::shared_ptr<fact> const & f);

    void pv_evaluate_facts_begin(int i);
    void pv_evaluate_facts_end(int i);
    void pv_evaluate_parent_begin(int i);
    void pv_evaluate_parent_end(int i);
    void pv_evaluate_end(int i);

    void pv_not(int i, fact_value l_value);
    void pv_and(int i, fact_value l_value, fact_value r_value);
    void pv_or(int i, fact_value l_value, fact_value r_value);
    void pv_xor(int i, fact_value l_value, fact_value r_value);

    void pv_imply_begin(int i, fact_value l_value, fact_value r_value);
    void pv_imply_totrue_begin(int i);
    void pv_imply_totrue_end(int i);

    void pv_totrue_fact(int i, std::shared_ptr<fact> const & fact);
    void pv_totrue_rule_begin(int i);
    void pv_totrue_rule_end(int i);
};
