#include "fact.hpp"
#include "options.hpp"

std::ostream & operator<<(std::ostream & os, fact_value const & rhs)
{
	switch (rhs)
	{
	case fact_value::TRUE:	os << (options::vm.count("color") ? "\033[32mTRUE\033[0m" : "TRUE"); break;
	default:				os << (options::vm.count("color") ? "\033[31mFALSE\033[0m" : "FALSE");
	}
	return os;
}

fact::fact()
	: name('0')
	, value(fact_value::FALSE)
	, rules{}
	, visited{false}
{}

fact::fact(char name)
	: name(name)
	, value(fact_value::FALSE)
	, rules{}
	, visited{false}
{}