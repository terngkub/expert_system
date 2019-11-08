#include "system.hpp"

expert_system::expert_system()
	: facts{}
	, rules{}
{
	// facts['A'] = std::make_shared<fact>(fact_value::FALSE);
	// facts['B'] = std::make_shared<fact>(fact_value::FALSE);
	// facts['C'] = std::make_shared<fact>(fact_value::FALSE);
	// facts['D'] = std::make_shared<fact>(fact_value::TRUE);
	// facts['E'] = std::make_shared<fact>(fact_value::TRUE);

	// rules.push_back(std::make_shared<rule>(rule_operation::AND, facts['B'], facts['C']));
	// facts['B']->rules.push_back(rules[0]);
	// facts['C']->rules.push_back(rules[0]);

	// rules.push_back(std::make_shared<rule>(rule_operation::IMPLY, rules[0], facts['A']));
	// facts['A']->rules.push_back(rules[1]);

	// rules.push_back(std::make_shared<rule>(rule_operation::XOR, facts['D'], facts['E']));
	// facts['D']->rules.push_back(rules[2]);
	// facts['E']->rules.push_back(rules[2]);

	// rules.push_back(std::make_shared<rule>(rule_operation::IMPLY, rules[2], facts['B']));
	// facts['B']->rules.push_back(rules[3]);

	// rules.push_back(std::make_shared<rule>(rule_operation::IMPLY, facts['B'], facts['C']));
	// facts['B']->rules.push_back(rules[4]);
	// facts['C']->rules.push_back(rules[4]);

	facts['A'] = std::make_shared<fact>(fact_value::TRUE);
	facts['B'] = std::make_shared<fact>(fact_value::FALSE);
	facts['C'] = std::make_shared<fact>(fact_value::FALSE);
	facts['D'] = std::make_shared<fact>(fact_value::FALSE);

	rules.push_back( std::make_shared<rule>( rule_operation::AND, facts['C'] ,facts['D'] ) );
	facts['C']->rules.push_back(rules[0]);
	facts['D']->rules.push_back(rules[0]);

	rules.push_back( std::make_shared<rule>( rule_operation::AND, facts['B'] ,rules[0] ) );
	facts['B']->rules.push_back(rules[1]);

	rules.push_back( std::make_shared<rule>( rule_operation::IMPLY, facts['A'] , rules[1] ) );
	facts['A']->rules.push_back(rules[2]);
}

void expert_system::operator()()
{
	query(facts['A']);
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