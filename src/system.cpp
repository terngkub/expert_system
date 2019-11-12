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

	queries = std::vector<char>{};

	std::cout << "Begin new iteration, every values are resetted to FALSE\n";
	std::cout << "Type \"exit\" (without quotes) to quit the program\n";
}

void expert_system::interactive_exit(std::string str)
{
	// improve exit parsing
	if (str == "exit")
	{
		std::cout << "Ending the program\n";
		std::exit(EXIT_SUCCESS);
	}
}

void expert_system::interactive_initial_facts()
{
	try
	{
		std::cout << "Please enter initial facts\n";
		std::string str{};
		getline(std::cin, str);
		interactive_exit(str);
		str = '=' + str;
		parser_.parse_initial_facts(str);
	}
	catch (std::exception & e)
	{
		std::cerr << "error: " << e.what() << '\n';
		interactive_initial_facts();
	}
}

void expert_system::interactive_query()
{
	try
	{
		std::cout << "Please enter query\n";
		std::string str{};
		getline(std::cin, str);
		interactive_exit(str);
		str = '?' + str;
		parser_.parse_query(str);
	}
	catch (std::exception & e)
	{
		std::cerr << "error: " << e.what() << '\n';
		interactive_query();
	}
}

void expert_system::interactive_loop()
{
	while(true)
	{
		run();
		reset();
		interactive_initial_facts();
		interactive_query();
	}
}

void expert_system::run()
{
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

void expert_system::operator()()
{
	parser_.parse();
	interactive_loop();
}

void expert_system::query(std::shared_ptr<fact> f)
{
	f->visited = true;
	for (auto r : f->rules)
	{
		r->evaluate();
	}
}
