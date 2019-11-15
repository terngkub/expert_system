#pragma once
#include "ast.hpp"
#include "rule.hpp"
#include <fstream>

struct expert_system;

class parser
{
public:
    // Constructors and Destructor
    parser() = delete;
    parser(expert_system & es, std::string & filename);
    ~parser();

    // Copy - Disable
    parser(parser const &) = delete;
    parser & operator=(parser const &) = delete;

    // Move - Disable
    parser(parser &&) = delete;
    parser & operator=(parser &&) = delete;

    // Function Object
    rule_node operator()(ast::rule &);
    rule_node operator()(ast::expr &);
    rule_node operator()(ast::operation &);
    rule_node operator()(ast::operand &);
    rule_node operator()(ast::signed_ &);
    rule_node operator()(char);

    // Parse File
    void parse();

    // Parse String
    void parse_initial_facts(std::string const & str);
    void parse_query(std::string const & str);

private:
    // Private Attributes
    expert_system & es;
    std::ifstream ifs;
    int rule_nb;

    // Line
    bool is_empty(std::string const & str);
    bool is_comment(std::string const & str);

    // Rules
    bool parse_rules(std::string const & str);
    std::shared_ptr<rule> create_new_rule(rule_operation operation, rule_node left, rule_node right);
    void link_rule(rule_node node, std::shared_ptr<rule> new_rule);

    // Initial Facts and Queries
    void set_initial_facts(std::vector<char> & result);
    void set_queries(std::vector<char> & result);
};