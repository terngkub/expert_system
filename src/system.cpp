#include "options.hpp"
#include "system.hpp"
#include <iostream>
#include <regex>
#include <string>

expert_system::expert_system(std::string & filename)
	: facts{}
	, rules{}
	, queries{}
	, parser_{*this, filename}
{}

void expert_system::operator()()
{
	parser_.parse();
	if (options::vm.count("interactive"))
		interactive_loop();
	else
		run();
}

void expert_system::run()
{
	for (auto const c : queries)
		query(facts[c]);

	if (options::vm.count("debug"))
		debug_print();
	else
		print();
}

void expert_system::print()
{
	for (auto c : queries)
	{
		std::cout << c << ": " << facts[c]->value << '\n';
	}
}

void expert_system::debug_print()
{
	std::cout << "Facts:\n";
	for (auto const & f : facts)
	{
		std::cout << f.first << ": " << f.second->value << '\n';
	}
	std::cout << "\nRules:\n";
	for (auto const & r : rules)
	{
		std::cout << r->operation << ": " << r->value << '\n';
	}
}

void expert_system::query(std::shared_ptr<fact> f)
{
	f->visited = true;
	for (auto r : f->rules)
	{
		r->evaluate();
	}
}

void expert_system::interactive_loop()
{
	std::cout
		<< "*************** INTERACTIVE MODE ***************\n\n"
		<< "You can change facts and query at each iteration\n"
		<< "Type \"exit\" (without quotes) to quit the program\n\n"
		<< "------------------------------------------------\n\n"
		<< "First iteration\n\n";

	while(true)
	{
		run();
		interactive_reset();
		interactive_initial_facts();
		interactive_query();
	}
}

void expert_system::interactive_reset()
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

	std::cout
		<< "\n------------------------------------------------\n\n"
		<< "Begin new iteration\n\n";
}

void expert_system::interactive_exit(std::string str)
{
	str = std::regex_replace(str, std::regex("(^\\s+)|(\\s+$)"),"");
	if (str == "exit")
	{
		std::cout << "\nClosing the program\nThank you for using our Expert System\n";
		std::exit(EXIT_SUCCESS);
	}
}

void expert_system::interactive_initial_facts()
{
	try
	{
		std::cout << "Enter initial facts: ";
		std::string str{};
		getline(std::cin, str);
		interactive_exit(str);
		str = '=' + str;
		parser_.parse_initial_facts(str);
		std::cout << '\n';
	}
	catch (std::exception & e)
	{
		std::cerr << "error: " << e.what() << "\n\n";
		interactive_initial_facts();
	}
}

void expert_system::interactive_query()
{
	try
	{
		std::cout << "Enter query: ";
		std::string str{};
		getline(std::cin, str);
		interactive_exit(str);
		str = '?' + str;
		parser_.parse_query(str);
		std::cout << '\n';
	}
	catch (std::exception & e)
	{
		std::cerr << "error: " << e.what() << "\n\n";
		interactive_query();
	}
}