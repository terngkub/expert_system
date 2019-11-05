#include "system.hpp"

expert_system::expert_system()
	: facts{}
	, rules{}
{
	facts['A'] = fact{fact_value::TRUE};
	facts['B'] = fact{fact_value::FALSE};
	facts['C'] = fact{fact_value::TRUE};

	rules.push_back(rule{rule_operation::AND, &facts['A'], &facts['B']});
	facts['A'].rules.push_back(&rules[0]);
	facts['B'].rules.push_back(&rules[0]);

	rules.push_back(rule{rule_operation::IMPLY, &rules[0], &facts['C']});
	facts['C'].rules.push_back(&rules[1]);
}

void expert_system::operator()()
{
	std::cout << query(&facts['C']) << '\n';
}

fact_value expert_system::query(fact * f)
{
	for (auto r : f->rules)
	{
		std::cout << "run " << r->operation << "\n";
		r->evaluate();
	}
	return f->value;
}