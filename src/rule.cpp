#include "options.hpp"
#include "utils.hpp"
#include "rule.hpp"
#include <variant>

// Rule Operation Enum

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


// Public Methods

void rule::evaluate(int i)
{
	if (visited) return;
	visited = true;

	pv_evaluate_facts_begin(i);

	auto l_value = get_fact_value(left, i + 1);
	auto r_value = (operation == rule_operation::NOT) ? fact_value::FALSE : get_fact_value(right, i + 1);

	pv_evaluate_facts_end(i);
	
	switch(operation)
	{
		case rule_operation::NOT: operation_not(l_value, i); break; 
		case rule_operation::AND: operation_and(l_value, r_value, i); break;
		case rule_operation::OR: operation_or(l_value, r_value, i); break;
		case rule_operation::XOR: operation_xor(l_value, r_value, i); break;
		default: operation_imply(l_value, r_value, i);
	}

	if (parent != nullptr && !parent->visited)
	{
		pv_evaluate_parent_begin(i);
		parent->evaluate(i + 1);
		pv_evaluate_parent_end(i);
	}

	pv_evaluate_end(i);
}

std::string rule::get_name(rule_node node)
{
	return std::visit(overloaded
		{
			[](std::shared_ptr<fact> f)
			{
				std::string name{f->name};
				return name;
			},
			[](std::shared_ptr<rule> r)
			{
				return "rule " + std::to_string(r->name);
			}
		},
		node);
}


// Private Methods


fact_value rule::get_fact_value(rule_node node, int i)
{
	return std::visit(overloaded
		{
			[this, i](std::shared_ptr<fact> f)
			{
				pv_getfactvalue_begin(i, f);

				if (!f->visited)
				{
					f->visited = true;

					std::vector<std::shared_ptr<rule>> rules{};
					for (auto r : f->rules)
					{
						if (!r->visited)
							rules.push_back(r);
					}

					pv_getfactvalue_evaluate_begin(i, rules);
					for (auto r : rules)
						r->evaluate(i + 1);
					pv_getfactvalue_evaluate_end(i, rules);
				}

				pv_getfactvalue_end(i, f);

				return f->value;
			},

			[i](std::shared_ptr<rule> r)
			{
				r->evaluate(i); 
				return r->value;
			}
		},
		node);
}

void rule::operation_not(fact_value l_value, int i)
{
	if (l_value == fact_value::TRUE)
		value = fact_value::FALSE;
	else
		value = fact_value::TRUE;

	pv_not(i, l_value);
}

void rule::operation_and(fact_value l_value, fact_value r_value, int i)
{
	if (l_value == fact_value::TRUE && r_value == fact_value::TRUE)
		value = fact_value::TRUE;
	else
		value = fact_value::FALSE;

	pv_and(i, l_value, r_value);
}

void rule::operation_or(fact_value l_value, fact_value r_value, int i)
{
	if (l_value == fact_value::TRUE || r_value == fact_value::TRUE)
		value = fact_value::TRUE;
	else
		value = fact_value::FALSE;

	pv_or(i, l_value, r_value);
}

void rule::operation_xor(fact_value l_value, fact_value r_value, int i)
{
	if (l_value != r_value)
		value = fact_value::TRUE;
	else
		value = fact_value::FALSE;

	pv_xor(i, l_value, r_value);
}

void rule::operation_imply(fact_value l_value, fact_value r_value, int i)
{
	pv_imply_begin(i, l_value, r_value);

	value = fact_value::TRUE;

	if (l_value == fact_value::TRUE && r_value == fact_value::FALSE)
	{
		pv_imply_totrue_begin(i);
		to_true(right, i + 1);
		pv_imply_totrue_end(i);
	}
}

void rule::to_true(rule_node node, int i)
{
	std::visit(overloaded
	{
		[this, i](std::shared_ptr<fact> f)
		{
			pv_totrue_fact(i, f);
			f->value = fact_value::TRUE;
		},
		[this, i](std::shared_ptr<rule> r)
		{
			pv_totrue_rule_begin(i);
			r->value = fact_value::TRUE;
			r->to_true(r->left, i + 1);
			r->to_true(r->right, i + 1);
			pv_totrue_rule_end(i);
		}
	},
	node);
}


// Print Evaluation

void rule::pv_getfactvalue_begin(int i, std::shared_ptr<fact> const & f)
{
	if (options::vm.count("visualisation"))
	{
		std::cout << indent(i) << hr << '\n';
		std::cout << indent(i) << "Trace back to fact " << f->name << '\n';
	}
}

void rule::pv_getfactvalue_evaluate_begin(int i, std::vector<std::shared_ptr<rule>> const & rules)
{
	if (options::vm.count("visualisation") && rules.size() > 0)
	{
		std::cout << indent(i) << "Linked rules:\n";
		for (auto r : rules)
		{
			if (r->operation == rule_operation::NOT)
				std::cout << indent(i) << "rule " << r->name << ": !" << r->get_name(r->left) << "\n";
			else
				std::cout << indent(i) << "rule " << r->name << ": " << r->get_name(r->left) << ' ' << r->operation << ' ' << r->get_name(r->right) << "\n";
		}
		std::cout << '\n';
	}
}

void rule::pv_getfactvalue_evaluate_end(int i, std::vector<std::shared_ptr<rule>> const & rules)
{
	if (options::vm.count("visualisation") && rules.size() > 0)
		std::cout << indent(i + 1) << hr << "\n\n";
}

void rule::pv_getfactvalue_end(int i, std::shared_ptr<fact> const & f)
{
	if (options::vm.count("visualisation"))
		std::cout << indent(i) << "Value : " << f->value << '\n';
}

void rule::pv_evaluate_facts_begin(int i)
{
	if (options::vm.count("visualisation"))
	{
		std::cout << indent(i) << hr << "\n";
		std::cout << indent(i) << "Trace back to rule " << name << "\n";
		if (operation == rule_operation::NOT)
			std::cout << indent(i) << "Desc   : !" << get_name(left) << "\n";
		else
			std::cout << indent(i) << "Desc   : " << get_name(left) << ' ' << operation << ' ' << get_name(right) << "\n";

		std::string parent_str = (parent != nullptr) ? "rule " + std::to_string(parent->name) : "None";
		std::cout << indent(i) << "Parent : " << parent_str << "\n\n";
	}
}

void rule::pv_evaluate_facts_end(int i)
{
	if (options::vm.count("visualisation"))
		std::cout << indent(i + 1) << hr << "\n\n";
}


void rule::pv_evaluate_parent_begin(int i)
{
	if (options::vm.count("visualisation"))
	{
		std::cout << indent(i) << "Value : " << value << "\n\n";
		std::cout << indent(i) << "Trace back to parent\n\n";
	}
}

void rule::pv_evaluate_parent_end(int i)
{
	if (options::vm.count("visualisation"))
		std::cout << indent(i + 1) << hr << "\n\n";
}

void rule::pv_evaluate_end(int i)
{
	if (options::vm.count("visualisation"))
		std::cout << indent(i) << "Value : " << value << '\n';
}

void rule::pv_not(int i, fact_value l_value)
{
	if (options::vm.count("visualisation"))
		std::cout << indent(i) << "Reasoning : !" << l_value << " == " << value << "\n";
}

void rule::pv_and(int i, fact_value l_value, fact_value r_value)
{
	if (options::vm.count("visualisation"))
		std::cout << indent(i) << "Reasoning : " << l_value << " + " << r_value << " == " << value << "\n";
}

void rule::pv_or(int i, fact_value l_value, fact_value r_value)
{
	if (options::vm.count("visualisation"))
		std::cout << indent(i) << "Reasoning : " << l_value << " | " << r_value << " == " << value << "\n";
}

void rule::pv_xor(int i, fact_value l_value, fact_value r_value)
{
	if (options::vm.count("visualisation"))
		std::cout << indent(i) << "Reasoning : " << l_value << " ^ " << r_value << " == " << value << "\n";
}

void rule::pv_imply_begin(int i, fact_value l_value, fact_value r_value)
{
	if (options::vm.count("visualisation"))
		std::cout << indent(i) << "Reasoning : " << l_value << " => " << r_value << "\n";
}

void rule::pv_imply_totrue_begin(int i)
{
	if (options::vm.count("visualisation"))
		std::cout << indent(i) << get_name(right) << " has to be " << fact_value::TRUE << "\n\n";
}

void rule::pv_imply_totrue_end(int i)
{
	if (options::vm.count("visualisation"))
		std::cout << indent(i + 1) << hr << "\n\n";
}

void rule::pv_totrue_fact(int i, std::shared_ptr<fact> const & f)
{

	if (options::vm.count("visualisation"))
	{
		std::cout << indent(i) << hr << "\n";
		std::cout << indent(i) << "Changing fact " << f->name << " to " << fact_value::TRUE << "\n";
	}
}

void rule::pv_totrue_rule_begin(int i)
{
	if (options::vm.count("visualisation"))
	{
		std::cout << indent(i) << hr << "\n";
		std::cout << indent(i) << "Changing rule " << name << " value to " << fact_value::TRUE << 
	"\n";
		std::cout << indent(i) << "Desc   : " << get_name(left) << ' ' << operation << ' ' << get_name(right) << "\n\n";
	}
}

void rule::pv_totrue_rule_end(int i)
{
	if (options::vm.count("visualisation"))
	{
		std::cout << indent(i + 1) << hr << "\n\n";
		std::cout << indent(i) << "Finished changing rule " << name << " values\n";
	}
}
