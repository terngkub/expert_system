#include "grammar.hpp"
#include "parser.hpp"
#include "system.hpp"
#include <regex>

parser::parser(expert_system & es, std::string & filename)
    : es{es}
    , ifs{}
{
	ifs.open(filename);
	if (ifs.fail())
		throw std::runtime_error("");
}

parser::~parser()
{
    ifs.close();
}

void parser::parse()
{
	using boost::spirit::x3::ascii::space;

	std::string line;
	int line_nb = 0;
    int rule_nb = 0;
    bool has_initial_facts = false;
    bool has_queries = false;

    enum grammar_type
    {
        RULE,
        INITIAL_FACTS,
        QUERIES
    };

    auto g = grammar_type::RULE;

	while (getline(ifs, line))
	{
		++line_nb;

        if (is_empty(line) || is_comment(line))
            continue;

        if (g == grammar_type::RULE)
        {
            ast::rule result;
            auto iter = line.cbegin();
            auto end = line.cend();
            bool r = phrase_parse(iter, end, grammar::input, space, result);
            if (r && iter == end)
            {
                (*this)(result);
                ++rule_nb;
                continue;
            }
            else
                g = grammar_type::INITIAL_FACTS;
        }
        if (g == grammar_type::INITIAL_FACTS)
        {
            std::vector<char> result;
            bool r = phrase_parse(line.cbegin(), line.cend(), grammar::initial_facts, space, result);
            set_initial_facts(result);
            if (r)
                has_initial_facts = true;
            g = grammar_type::QUERIES;
        }
        else if (g == grammar_type::QUERIES)
        {
            std::vector<char> result;
            bool r = phrase_parse(line.cbegin(), line.cend(), grammar::queries, space, result);
            if (r)
                has_queries = true;
            set_queries(result);
        }
	}
    if (rule_nb == 0)
        throw std::runtime_error("there is no rules in the input");
    if (!has_initial_facts)
        throw std::runtime_error("there is no initial facts in the input");
    if (!has_queries)
        throw std::runtime_error("there is no queries in the input");
    
}

void parser::set_initial_facts(std::vector<char> & result)
{
    // TODO handle duplication case
    for (auto const c : result)
    {
        es.facts[c]->value = fact_value::TRUE;
    }
}

void parser::set_queries(std::vector<char> & result)
{
    // TODO handle duplication case
    for (auto const c : result)
    {
        es.queries.push_back(c);
    }
}

bool parser::is_empty(std::string const & str)
{
    if (str.empty())
        return true;
    std::smatch matches;
    std::regex pattern{"^\\s.*$"};
    return std::regex_search(str, matches, pattern);
}

bool parser::is_comment(std::string const & str)
{
    std::smatch matches;
    std::regex pattern{"^\\s*#.*$"};
    return std::regex_search(str, matches, pattern);
}

rule_node parser::operator()(char c)
{
    if (es.facts.find(c) == es.facts.end())
    {
        es.facts[c] = std::make_shared<fact>();
    }
    return es.facts[c];
}

rule_node parser::operator()(ast::operand & operand)
{
    return boost::apply_visitor(*this, operand);
}

rule_node parser::operator()(ast::signed_ & signed_)
{
    auto left = (*this)(signed_.operand_);

    if (signed_.sign != '!')
        return left;

    auto right = std::make_shared<fact>();
    auto new_rule = std::make_shared<rule>(rule_operation::NOT, left, right);
    es.rules.push_back(new_rule);

    if (std::holds_alternative<std::shared_ptr<fact>>(left))
    {
        auto l = std::get<std::shared_ptr<fact>>(left);
        l->rules.push_back(new_rule);
    }

    return new_rule;
}

rule_node parser::operator()(ast::left_expr & left_expr)
{
    auto left = (*this)(left_expr.first);
    if (left_expr.rest.size() == 0)
        return left;

    std::shared_ptr<rule> new_rule;

    for (auto & opt : left_expr.rest)
    {
        auto right = (*this)(opt.operand_);
        rule_operation operation_value;
        switch (opt.operator_)
        {
            case '+': operation_value = rule_operation::AND; break;
            case '|': operation_value = rule_operation::OR; break;
            default: operation_value = rule_operation::XOR;
        }
        new_rule = std::make_shared<rule>(operation_value, left, right);
        es.rules.push_back(new_rule);

        if (std::holds_alternative<std::shared_ptr<fact>>(left))
        {
            auto l = std::get<std::shared_ptr<fact>>(left);
            l->rules.push_back(new_rule);
        }

        if (std::holds_alternative<std::shared_ptr<fact>>(right))
        {
            auto r = std::get<std::shared_ptr<fact>>(right);
            r->rules.push_back(new_rule);
        }
    }
    return new_rule;
}

rule_node parser::operator()(ast::right_expr & right_expr)
{
    auto left = (*this)(right_expr.first);
    if (right_expr.rest.size() == 0)
        return left;

    std::shared_ptr<rule> new_rule;

    for (auto & opt : right_expr.rest)
    {
        auto right = (*this)(opt.operand_);
        rule_operation operation_value;
        switch (opt.operator_)
        {
            case '+': operation_value = rule_operation::AND; break;
            case '|': operation_value = rule_operation::OR; break;
            default: operation_value = rule_operation::XOR;
        }
        new_rule = std::make_shared<rule>(operation_value, left, right);
        es.rules.push_back(new_rule);

        if (std::holds_alternative<std::shared_ptr<fact>>(left))
        {
            auto l = std::get<std::shared_ptr<fact>>(left);
            l->rules.push_back(new_rule);
        }

        if (std::holds_alternative<std::shared_ptr<fact>>(right))
        {
            auto r = std::get<std::shared_ptr<fact>>(right);
            r->rules.push_back(new_rule);
        }
    }
    return new_rule;
}

rule_node parser::operator()(ast::rule & r)
{
    auto left = (*this)(r.left);
    auto right = (*this)(r.right);

    auto new_rule = std::make_shared<rule>(rule_operation::IMPLY, left, right);
    es.rules.push_back(new_rule);

    if (std::holds_alternative<std::shared_ptr<fact>>(left))
    {
        auto l = std::get<std::shared_ptr<fact>>(left);
	    l->rules.push_back(new_rule);
    }

    if (std::holds_alternative<std::shared_ptr<fact>>(right))
    {
        auto r = std::get<std::shared_ptr<fact>>(right);
	    r->rules.push_back(new_rule);
    }
    return new_rule;
}
