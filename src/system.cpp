#include "system.hpp"
#include <string>
#include <iostream>

expert_system::expert_system(std::string & filename)
	: facts{}
	, rules{}
	, queries{}
	, parser_{*this, filename}
{}

void expert_system::reset()
{
	for (auto & f : facts)
	{
		f.second->value = fact_value::FALSE;
		f.second->visited = false;
	}
	for (auto & r : rules)
	{
		r->value = fact_value::FALSE;
		r->visited = false;
	}
}

/*
void expert_system::interactive_loop()
{
	while()
	{
		// run the first time
		std::cout
			<< "Begin new iteration\n"
			<< "All the fact values are set to FALSE\n"
			<< "Please enter the fact symbols that you want to be TRUE\n"
			<< "=";
		std::string fact_string;


	}
}
*/


void expert_system::operator()()
{
	parser_.parse();
	for (auto const c : queries)
	{
		query(facts[c]);
	}

	// TODO only print fact value for queried facts
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
