#pragma once
#include "ast.hpp"
#include "rule.hpp"
#include <fstream>

struct expert_system;

struct parser
{
    expert_system & es;
    std::ifstream ifs;

    parser(expert_system & es, std::string & filename);
    ~parser();

    void parse();
    bool is_empty(std::string const & str);
    bool is_comment(std::string const & str);
    void set_initial_facts(std::vector<char> & result);
    void set_queries(std::vector<char> & result);

    rule_node operator()(char);
    rule_node operator()(ast::operand &);
    rule_node operator()(ast::signed_ &);
    rule_node operator()(ast::operation &);
    rule_node operator()(ast::left_expr &);
    rule_node operator()(ast::right_expr &);
    rule_node operator()(ast::rule &);
};