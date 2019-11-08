#include "system.hpp"
#include <string>
#include <iostream>

expert_system::expert_system(std::string & filename)
	: facts{}
	, rules{}
	, parser_{*this, filename}
{}

void expert_system::operator()()
{
	parser_.parse();
	
	facts['P']->value = fact_value::TRUE;


	query(facts['P']);
	for (auto const & f : facts)
	{
		std::cout << f.first << ": " << f.second->value << '\n';
	}
}

fact_value expert_system::query(std::shared_ptr<fact> f)
{
	for (auto r : f->rules)
	{
		r->evaluate();
	}
	return f->value;
}
