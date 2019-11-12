#pragma once
#include "ast.hpp"
#include "rule.hpp"
#include <fstream>

struct expert_system;

struct parser
{
    expert_system & es;
    std::ifstream ifs;
    int rule_nb;

    parser(expert_system & es, std::string & filename);
    ~parser();

    void parse();
    bool is_empty(std::string const & str);
    bool is_comment(std::string const & str);

    bool parse_rules(std::string const & str);
    void parse_initial_facts(std::string const & str);
    void parse_query(std::string const & str);

    void set_initial_facts(std::vector<char> & result);
    void set_queries(std::vector<char> & result);

    void link_rule(rule_node node, std::shared_ptr<rule> new_rule);
    std::shared_ptr<rule> create_new_rule(rule_operation operation, rule_node left, rule_node right);

    rule_node operator()(char);
    rule_node operator()(ast::operand &);
    rule_node operator()(ast::signed_ &);
    rule_node operator()(ast::operation &);
    rule_node operator()(ast::expr &);
    rule_node operator()(ast::rule &);
};