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

fact_value rule::get_fact_value(rule_node node)
{
	return std::visit(overloaded
		{
			[](fact * f) { return f->value; },
			[](rule * r) { return r->evaluate(); }
		},
		node);
}

fact_value rule::evaluate()
{
	auto l_value = get_fact_value(left);
	auto r_value = get_fact_value(right);
	return (l_value == fact_value::TRUE && r_value == fact_value::TRUE) ? fact_value::TRUE : fact_value::FALSE;
}

