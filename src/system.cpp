#include "system.hpp"
#include <string>
#include <iostream>

expert_system::expert_system(std::string & filename)
	: facts{}
	, rules{}
	, queries{}
	, parser_{*this, filename}
{}

void expert_system::operator()()
{
	parser_.parse();
	for (auto const c : queries)
	{
		query(facts[c]);
	}
	for (auto const & f : facts)
	{
		std::cout << f.first << ": " << f.second->value << '\n';
	}
}

fact_value expert_system::query(std::shared_ptr<fact> f)
{
	f->visited = true;
	for (auto r : f->rules)
	{
		r->evaluate();
	}
	return f->value;
}
