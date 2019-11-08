#include "grammar.hpp"
#include "parser.hpp"
#include "system.hpp"

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
	int line_nb = 1;

	while (getline(ifs, line))
	{
		auto iter = line.cbegin();
		auto const end = line.cend();
		ast::rule result;
		
		bool r = phrase_parse(iter, end, grammar::input, space, result);
		if (r && iter == end)
		{
			std::cout << "line " << line_nb << " parsed successfully\n";
			(*this)(result);
		}
		else
		{
			std::string rest(iter, end);
			throw std::runtime_error("parsing fail at: " + rest);
		}
		++line_nb;
	}
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
        // auto old_rule = new_rule;
        new_rule = std::make_shared<rule>(operation_value, left, right);
        es.rules.push_back(new_rule);
        // old_rule->parent = new_rule;

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

        // left = new_rule;
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

        // auto old_rule = new_rule;
        new_rule = std::make_shared<rule>(operation_value, left, right);
        es.rules.push_back(new_rule);
        // old_rule->parent = new_rule;

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

        // left = new_rule;
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
