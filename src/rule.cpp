#include "rule.hpp"
#include <variant>

std::ostream & operator<<(std::ostream & os, rule_operation const & rhs)
{
	switch (rhs)
	{
	case NOT:	os << "NOT"; break;
	case AND:	os << "AND"; break;
	case OR:	os << "OR"; break;
	case XOR:	os << "XOR"; break;
	case IMPLY:	os << "IMPLY"; break;
	default:	os << "IAOF"; break;
	}
	return os;
}

fact_value * rule::get_fact_value(rule_node node)
{
	return std::visit(overloaded
		{
			[](std::shared_ptr<fact> f)
			{ 
				if (f->visited)
					return &(f->value);
				f->visited = true;

				for (auto r : f->rules)
				{
					r->evaluate();
				}
				return &(f->value);
			},
			[](std::shared_ptr<rule> r) {
				if (r->visited)
					return &(r->value);
				r->visited = true;
					
				r->evaluate(); 
				return &(r->value);
			}
		},
		node);
}

void rule::evaluate()
{
	auto l_value = get_fact_value(left);
	auto r_value = get_fact_value(right);
	
	if (*r_value == fact_value::NONE && operation != NOT)
		std::cout << "/!\\ THERE IS A ISSUE /!\\ \n";
	switch(operation)
	{
		case rule_operation::AND: operation_and(l_value, r_value); break;
		case rule_operation::OR: operation_or(l_value, r_value); break;
		case rule_operation::XOR: operation_xor(l_value, r_value); break;
		case rule_operation::NOT: operation_not(l_value, r_value); break; 
		default: operation_imply(l_value, r_value, right);
	}
}

void rule::operation_and(fact_value * l_value, fact_value * r_value)
{
	std::cout << "and: " << *l_value << " " << *r_value << ' ' << visited << '\n';
	if (*l_value == fact_value::TRUE && *r_value == fact_value::TRUE)
		value = fact_value::TRUE;
	else
		value = fact_value::FALSE;
}

void rule::operation_or(fact_value * l_value, fact_value * r_value)
{
	std::cout << "or: " << *l_value << " " << *r_value << ' ' << visited << '\n';
	if (*l_value == fact_value::TRUE || *r_value == fact_value::TRUE)
		value = fact_value::TRUE;
	else
		value = fact_value::FALSE;

}

void rule::operation_imply(fact_value * l_value, fact_value * r_value, rule_node right)
{
	std::cout << "imply: " << *l_value << " " << *r_value << ' ' << visited << '\n';
	if (*l_value == fact_value::TRUE && *r_value == fact_value::FALSE)
	{
		*r_value = fact_value::TRUE;

		//re-evaluate right if it is a facts
		if ( std::holds_alternative<std::shared_ptr<fact>>(right) )
		{
			auto name = std::get<std::shared_ptr<fact>>(right);
			for (auto r : name->rules)
			{
				r->evaluate();
			}
		}
	}
	if ( std::holds_alternative<std::shared_ptr<rule>>(right) )
	{
		auto name = std::get<std::shared_ptr<rule>>(right);
		if (name->operation == rule_operation::AND && *l_value == fact_value::TRUE && *r_value == fact_value::TRUE)
		{
			// CHANGE RIGHT FACTS TO TRUE
			// REPEAT IF RIGHT->RIGHT IS RULE AND && RIGHT->LEFT IS TRUE
		}
	}
}

void rule::operation_xor(fact_value * l_value, fact_value * r_value)
{
	std::cout << "xor: " << *l_value << " " << *r_value << ' ' << visited << '\n';
	if ( ( *l_value == fact_value::TRUE && *r_value == fact_value::FALSE ) 
	||  ( *l_value == fact_value::FALSE && *r_value == fact_value::TRUE)  )
		value = fact_value::TRUE;
	else
		value = fact_value::FALSE;
}

void rule::operation_not(fact_value * l_value, fact_value * r_value)
{
	std::cout << "not: " << *l_value << " " << *r_value << ' ' << visited << '\n';
	if (*l_value == fact_value::TRUE)
		value = fact_value::FALSE;
	else
		value = fact_value::TRUE;
}