#include "system.hpp"
#include <iostream>
#include <fstream>

std::string parse_args(int argc, char ** argv)
{
    if (argc != 2)
        throw std::runtime_error("Usage ./expert_system [file]\n");

    std::string filename = std::string(argv[1]);

    if (filename.empty())
        throw std::runtime_error("file name is empty");

    return filename;
}

int main(int argc, char ** argv)
{
    try
    {
        auto filename = parse_args(argc, argv);

        expert_system{filename}();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}