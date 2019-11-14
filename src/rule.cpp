#include "options.hpp"
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

fact_value * rule::get_fact_value(rule_node node, int i)
{
	return std::visit(overloaded
		{
			[i](std::shared_ptr<fact> f)
			{
				if (options::vm.count("visualisation"))
				{
					std::cout << indent(i) << hr << '\n';
					std::cout << indent(i) << "Trace back to fact " << f->name << '\n';
				}

				if (!f->visited)
				{
					f->visited = true;

					std::vector<std::shared_ptr<rule>> rules{};

					for (auto r : f->rules)
					{
						if (!r->visited)
							rules.push_back(r);
					}

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

					for (auto r : rules)
					{
						r->evaluate(i + 1);
					}

					if (options::vm.count("visualisation") && rules.size() > 0)
					{
						std::cout << indent(i + 1) << hr << "\n\n";
					}
				}

				if (options::vm.count("visualisation"))
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

void rule::evaluate(int i)
{
	if (visited) return;
	visited = true;

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

	auto l_value = get_fact_value(left, i + 1);
	auto r_value = (operation == rule_operation::NOT) ? nullptr : get_fact_value(right, i + 1);

	if (options::vm.count("visualisation"))
		std::cout << indent(i + 1) << hr << "\n\n";
	
	switch(operation)
	{
		case rule_operation::AND: operation_and(l_value, r_value, i); break;
		case rule_operation::OR: operation_or(l_value, r_value, i); break;
		case rule_operation::XOR: operation_xor(l_value, r_value, i); break;
		case rule_operation::NOT: operation_not(l_value, r_value, i); break; 
		default: operation_imply(l_value, r_value, i);
	}

	if (parent != nullptr && !parent->visited)
	{
		if (options::vm.count("visualisation"))
		{
			std::cout << indent(i) << "Value : " << value << "\n\n";
			std::cout << indent(i) << "Trace back to parent\n\n";
		}

		parent->evaluate(i + 1);

		if (options::vm.count("visualisation"))
			std::cout << indent(i + 1) << hr << "\n\n";
	}

	if (options::vm.count("visualisation"))
		std::cout << indent(i) << "Value : " << value << '\n';
}

void rule::operation_not(fact_value * l_value, fact_value * r_value, int i)
{
	if (*l_value == fact_value::TRUE)
		value = fact_value::FALSE;
	else
		value = fact_value::TRUE;

	if (options::vm.count("visualisation"))
		std::cout << indent(i) << "Reasoning : !" << *l_value << " == " << value << "\n";
}

void rule::operation_and(fact_value * l_value, fact_value * r_value, int i)
{
	if (*l_value == fact_value::TRUE && *r_value == fact_value::TRUE)
		value = fact_value::TRUE;
	else
		value = fact_value::FALSE;

	if (options::vm.count("visualisation"))
		std::cout << indent(i) << "Reasoning : " << *l_value << " + " << *r_value << " == " << value << "\n";
}

void rule::operation_or(fact_value * l_value, fact_value * r_value, int i)
{
	if (*l_value == fact_value::TRUE || *r_value == fact_value::TRUE)
		value = fact_value::TRUE;
	else
		value = fact_value::FALSE;

	if (options::vm.count("visualisation"))
		std::cout << indent(i) << "Reasoning : " << *l_value << " | " << *r_value << " == " << value << "\n";
}

void rule::operation_xor(fact_value * l_value, fact_value * r_value, int i)
{
	if ( ( *l_value == fact_value::TRUE && *r_value == fact_value::FALSE ) 
	||  ( *l_value == fact_value::FALSE && *r_value == fact_value::TRUE)  )
		value = fact_value::TRUE;
	else
		value = fact_value::FALSE;

	if (options::vm.count("visualisation"))
		std::cout << indent(i) << "Reasoning : " << *l_value << " ^ " << *r_value << " == " << value << "\n";
}

void rule::operation_imply(fact_value * l_value, fact_value * r_value, int i)
{
	if (options::vm.count("visualisation"))
	std::cout << indent(i) << "Reasoning : " << *l_value << " => " << *r_value << "\n";

	value = fact_value::TRUE;
	if (*l_value == fact_value::TRUE && *r_value == fact_value::FALSE)
	{
		if (options::vm.count("visualisation"))
			std::cout << indent(i) << get_name(right) << " has to be TRUE\n\n";

		to_true(right, i + 1);

		if (options::vm.count("visualisation"))
			std::cout << indent(i + 1) << hr << "\n\n";
	}
}

void rule::to_true(rule_node node, int i)
{
	std::visit(overloaded
	{
		[this, i](std::shared_ptr<fact> f)
		{
			if (options::vm.count("visualisation"))
			{
				std::cout << indent(i) << hr << "\n";
				std::cout << indent(i) << "Changing fact " << f->name << " to TRUE\n";
			}

			f->value = fact_value::TRUE;
		},
		[this, i](std::shared_ptr<rule> r)
		{
			if (options::vm.count("visualisation"))
			{
				std::cout << indent(i) << hr << "\n";
				std::cout << indent(i) << "Changing rule " << name << " value to TRUE\n";
				std::cout << indent(i) << "Desc   : " << get_name(left) << ' ' << operation << ' ' << get_name(right) << "\n\n";
			}

			r->value = fact_value::TRUE;
			r->to_true(r->left, i + 1);
			r->to_true(r->right, i + 1);

			if (options::vm.count("visualisation"))
			{
				std::cout << indent(i + 1) << hr << "\n\n";
				std::cout << indent(i) << "Finished changing rule " << name << " values\n";
			}
		}
	},
	node);
}

