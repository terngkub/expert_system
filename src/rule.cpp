#include "rule.hpp"
#include <variant>

std::ostream & operator<<(std::ostream & os, rule_operation const & rhs)
{
	switch (rhs)
	{
	case NOT:	os << "!"; break;
	case AND:	os << "+"; break;
	case OR:	os << "|"; break;
	case XOR:	os << "^"; break;
	default:	os << "=>";
	}
	return os;
}

std::string indent(int i)
{
	std::string str{};
	for (auto j = 0; j < i; ++j)
		str += "    ";
	return str;
}

const std::string hr{"------------------------------"};

fact_value * rule::get_fact_value(rule_node node, int i)
{
	return std::visit(overloaded
		{
			[i](std::shared_ptr<fact> f)
			{
				std::cout << indent(i) << hr << '\n';
				std::cout << indent(i) << "Trace back to fact " << f->name << '\n';

				if (!f->visited)
				{
					f->visited = true;
					for (auto r : f->rules)
						r->evaluate(i + 1);
				}

				std::cout << indent(i) << "Value : " << f->value << '\n';

				return &(f->value);
			},

			[i](std::shared_ptr<rule> r)
			{
				r->evaluate(i); 
				return &(r->value);
			}
		},
		node);
}

void rule::evaluate(int i)
{
	if (visited) return;
	visited = true;

	std::cout << indent(i) << hr << "\n";
	std::cout << indent(i) << "Trace back to rule " << name << "\n";
	std::cout << indent(i) << "Desc   : \n";
	std::string parent_str = (parent != nullptr) ? "rule " + std::to_string(parent->name) : "None";
	std::cout << indent(i) << "Parent : " << parent_str << "\n\n";

	auto l_value = get_fact_value(left, i + 1);
	auto r_value = get_fact_value(right, i + 1);

	std::cout << indent(i + 1) << hr << "\n\n";
	
	switch(operation)
	{
		case rule_operation::AND: operation_and(l_value, r_value, i); break;
		case rule_operation::OR: operation_or(l_value, r_value, i); break;
		case rule_operation::XOR: operation_xor(l_value, r_value, i); break;
		case rule_operation::NOT: operation_not(l_value, r_value, i); break; 
		default: operation_imply(l_value, r_value, i);
	}

	if (parent != nullptr)
	{
		if (!parent->visited)
		{
			std::cout << indent(i) << "Value : " << value << "\n\n";
			std::cout << indent(i) << "Trace back to parent\n\n";
		}

		parent->evaluate(i + 1);
	}

	std::cout << indent(i) << "Value : " << value << '\n';
}

void rule::operation_not(fact_value * l_value, fact_value * r_value, int i)
{
	if (*l_value == fact_value::TRUE)
		value = fact_value::FALSE;
	else
		value = fact_value::TRUE;

	std::cout << indent(i) << "Reasoning : !" << *l_value << " == " << value << "\n";
}

void rule::operation_and(fact_value * l_value, fact_value * r_value, int i)
{
	if (*l_value == fact_value::TRUE && *r_value == fact_value::TRUE)
		value = fact_value::TRUE;
	else
		value = fact_value::FALSE;

	std::cout << indent(i) << "Reasoning : " << *l_value << " + " << *r_value << " == " << value << "\n";
}

void rule::operation_or(fact_value * l_value, fact_value * r_value, int i)
{
	std::cout << indent(i) << "OR\n";
	if (*l_value == fact_value::TRUE || *r_value == fact_value::TRUE)
		value = fact_value::TRUE;
	else
		value = fact_value::FALSE;

	std::cout << indent(i) << "Reasoning : " << *l_value << " | " << *r_value << " == " << value << "\n";
}

void rule::operation_xor(fact_value * l_value, fact_value * r_value, int i)
{
	if ( ( *l_value == fact_value::TRUE && *r_value == fact_value::FALSE ) 
	||  ( *l_value == fact_value::FALSE && *r_value == fact_value::TRUE)  )
		value = fact_value::TRUE;
	else
		value = fact_value::FALSE;

	std::cout << indent(i) << "Reasoning : " << *l_value << " ^ " << *r_value << " == " << value << "\n";
}

void rule::operation_imply(fact_value * l_value, fact_value * r_value, int i)
{
	std::cout << indent(i) << "Reasoning : " << *l_value << " => " << *r_value << "\n";

	if (*l_value == fact_value::TRUE && *r_value == fact_value::FALSE)
	{
		std::cout << indent(i) << "XX has to be TRUE\n";
		to_true(right);
	}
}

void rule::to_true(rule_node node)
{
	std::visit(overloaded
	{
		[](std::shared_ptr<fact> f)
		{
			f->value = fact_value::TRUE;
		},
		[](std::shared_ptr<rule> r)
		{
			r->value = fact_value::TRUE;
			r->to_true(r->left);
			r->to_true(r->right);
		}
	},
	node);
}

