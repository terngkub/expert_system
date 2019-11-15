#include "grammar.hpp"
#include "parser.hpp"
#include "expert_system.hpp"
#include <regex>

// Constructor and Destructors

parser::parser(expert_system & es, std::string & filename)
	: es{es}
	, ifs{}
	, rule_nb{0}
{
	ifs.open(filename);
	if (ifs.fail())
		throw std::runtime_error("fail to open " + filename);
}

parser::~parser()
{
	ifs.close();
}


// Function Objects

rule_node parser::operator()(ast::rule & r)
{
	auto left = (*this)(r.left);
	auto right = (*this)(r.right);

	return create_new_rule(rule_operation::IMPLY, left, right);
}

rule_node parser::operator()(ast::expr & expr)
{
	auto left = (*this)(expr.first);

	for (auto & opt : expr.rest)
	{
		auto right = (*this)(opt.operand_);

		rule_operation operation_value;
		switch (opt.operator_)
		{
			case '+': operation_value = rule_operation::AND; break;
			case '|': operation_value = rule_operation::OR; break;
			default: operation_value = rule_operation::XOR;
		}

		left = create_new_rule(operation_value, left, right);
	}
	return left;
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

	return create_new_rule(rule_operation::NOT, left, right);
}

rule_node parser::operator()(char c)
{
	if (es.facts.find(c) == es.facts.end())
	{
		es.facts[c] = std::make_shared<fact>(c);
	}
	return es.facts[c];
}


// Parse File

void parser::parse()
{
	using boost::spirit::x3::ascii::space;

	std::string line;
	int rule_nb = 0;

	enum grammar_type
	{
		RULE,
		INITIAL_FACTS,
		QUERY,
		DONE
	};

	auto g = grammar_type::RULE;

	while (getline(ifs, line))
	{
		if (is_empty(line) || is_comment(line))
			continue;

		if (g == grammar_type::RULE)
		{
			if (parse_rules(line))
			{
				++rule_nb;
				continue;
			}
			g = grammar_type::INITIAL_FACTS;
		}
		if (g == grammar_type::INITIAL_FACTS)
		{
			if (rule_nb == 0)
				throw std::runtime_error("there is no rules in the input");

			parse_initial_facts(line);
			g = grammar_type::QUERY;
		}
		else if (g == grammar_type::QUERY)
		{
			parse_query(line);
			g = grammar_type::DONE;
		}
		else
			throw std::runtime_error("the input doesn't end after query");
	}
	if (rule_nb == 0)
		throw std::runtime_error("there is no rules in the input");
	if (g == grammar_type::RULE || g == grammar_type::INITIAL_FACTS)
		throw std::runtime_error("there is no initial facts in the input");
	if (g == grammar_type::QUERY)
		throw std::runtime_error("there is no query in the input");
}


// Parse String

void parser::parse_initial_facts(std::string const & str)
{
	auto it = str.begin();
	auto end = str.end();
	std::vector<char> result;

	bool r = phrase_parse(it, end, grammar::initial_facts, boost::spirit::x3::space, result);

	if (!r || it != end)
		throw std::runtime_error("invalid initial facts");

	set_initial_facts(result);
}

void parser::parse_query(std::string const & str)
{
	auto it = str.begin();
	auto end = str.end();
	std::vector<char> result;

	bool r = phrase_parse(it, end, grammar::queries, boost::spirit::x3::ascii::space, result);

	if (!r || it != end)
		throw std::runtime_error("invalid query");

	set_queries(result);
}


// Line

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


// Rules

bool parser::parse_rules(std::string const & str)
{
	auto it = str.begin();
	auto end = str.end();
	ast::rule result;

	bool r = phrase_parse(it, end, grammar::input, boost::spirit::x3::ascii::space, result);

	if (r)
	{
		if (it != end)
			throw std::runtime_error("invalid rule");

		(*this)(result);
		return true;
	}

	return false;
}

std::shared_ptr<rule> parser::create_new_rule(rule_operation operation_value, rule_node left, rule_node right)
{
	auto new_rule = std::make_shared<rule>(++rule_nb, operation_value, left, right);
	es.rules.push_back(new_rule);

	link_rule(left, new_rule);
	link_rule(right, new_rule);

	return new_rule;
}

void parser::link_rule(rule_node node, std::shared_ptr<rule> new_rule)
{
	std::visit(overloaded
	{
		[new_rule](std::shared_ptr<rule> l)
		{
			if (l == nullptr)
				return;
			l->parent = new_rule;
		},
		[new_rule](std::shared_ptr<fact> l)
		{
			if (l == nullptr)
				return;
			l->rules.push_back(new_rule);
		}
	},
	node);
}


// Parse Initial Facts and Queries

void parser::set_initial_facts(std::vector<char> & result)
{
	for (auto const c : result)
	{
		if (es.facts.find(c) == es.facts.end())
			throw std::runtime_error("there is no facts in the graph for this initial facts");
		es.facts[c]->value = fact_value::TRUE;
	}
}

void parser::set_queries(std::vector<char> & result)
{
	for (auto const c : result)
	{
		if (es.facts.find(c) == es.facts.end())
			throw std::runtime_error("there is no facts in the graph for this query");
		if (std::find(es.queries.begin(), es.queries.end(), c) != es.queries.end())
			continue;
		es.queries.push_back(c);
	}
}