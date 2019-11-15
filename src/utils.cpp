#include "utils.hpp"

std::string indent(int i)
{
	std::string str{};
	for (auto j = 0; j < i; ++j)
		str += "    ";
	return str;
}