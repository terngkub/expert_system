#include "fact.hpp"

std::ostream & operator<<(std::ostream & os, fact_value const & rhs)
{
	switch (rhs)
	{
	case fact_value::TRUE:	os << "TRUE"; break;
	case fact_value::FALSE:	os << "FALSE"; break;
	default:				os << "UNDETERMINED"; break;
	}
	return os;
}

fact::fact()
	: value(fact_value::FALSE)
	, rules{}
{}

fact::fact(fact_value value)
	: value(value)
	, rules{}
{}