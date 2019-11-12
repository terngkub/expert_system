#include "fact.hpp"

std::ostream & operator<<(std::ostream & os, fact_value const & rhs)
{
	switch (rhs)
	{
	case fact_value::TRUE:	os << "TRUE"; break;
	default:				os << "FALSE";
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