#include "utils.hpp"
#include <iostream>

std::string indent(int i)
{
	std::string str{};
	for (auto j = 0; j < i; ++j)
		str += "    ";
	return str;
}

void handle_eof()
{
	if (std::cin.eof())
	{
		std::cerr << "\n\nGetting EOF\nClosing the program\n";
		exit(EXIT_FAILURE);
	}
}