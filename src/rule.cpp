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

	// if right is a rule && is AND operator && left is true, 
	//right->right and right->left must be set to true &&
	//re-evaluate changed facts
	if ( std::holds_alternative<std::shared_ptr<rule>>(right) )
	{
		auto name = std::get<std::shared_ptr<rule>>(right);
		fact_value * tmpl_value = l_value;
		fact_value * tmpr_value = r_value;
		rule_node	tmp_right 	 = right;
	std::cout << " LEFT: "<< *tmpl_value << " RIGHT: " << *tmpr_value << '\n';
		while (name->operation == rule_operation::AND && *tmpl_value == fact_value::TRUE && *tmpr_value == fact_value::TRUE )
		{
			std::cout << "\nhello !!!!\n";
			// CHANGE RIGHT FACTS TO TRUE
			// REPEAT IF RIGHT->RIGHT IS RULE AND && RIGHT->LEFT IS TRUE
			
			tmpl_value = name->get_fact_value(name->left);
			tmpr_value = name->get_fact_value(name->right);
			std::cout << " LEFT: "<< *tmpl_value << " RIGHT: " << *tmpr_value << '\n';
			*tmpl_value = fact_value::TRUE;
			*tmpr_value = fact_value::TRUE;

			tmp_right = name->right; 
			if ( std::holds_alternative<std::shared_ptr<rule>>(tmp_right) )
			{
				name = std::get<std::shared_ptr<rule>>(tmp_right);
				std::cout << name->operation;
			}
			else
				break;
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

/*
L + M => K
O + P => L + N
N => M
*/