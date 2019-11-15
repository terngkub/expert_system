#include "options.hpp"
#include "expert_system.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char ** argv)
{
    try
    {
        options::get_options(argc, argv);
        if (options::vm.count("help") || !options::vm.count("filename"))
        {
            std::cout << "Usage: expert_system [filename]\n";
            std::cout << options::visible;
            return EXIT_FAILURE;
        }
        if (options::vm["filename"].as<std::string>().empty())
        {
            throw std::runtime_error("filename is empty");
        }

        std::string filename = options::vm["filename"].as<std::string>();
        expert_system{filename}();
    }
    catch(const std::exception& e)
    {
        std::cerr << "error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}