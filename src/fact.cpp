#include "fact.hpp"

std::ostream & operator<<(std::ostream & os, fact_value const & rhs)
{
	switch (rhs)
	{
	case fact_value::TRUE:	os << "TRUE"; break;
	case fact_value::FALSE:	os << "FALSE"; break;
	case fact_value::NONE:	os << "NONE"; break;
	default:				os << "UNDETERMINED"; break;
	}
	return os;
}

fact::fact()
	: value(fact_value::FALSE)
	, rules{}
	, visited{false}
{}

fact::fact(fact_value value)
	: value(value)
	, rules{}
	, visited{false}
{}

fact::fact(fact_value value, std::shared_ptr<rule> r)
	: value(value)
	, rules{r}
	, visited{false}
{}