#include "options.hpp"
#include <iostream>

boost::program_options::options_description options::visible{"Options"};
boost::program_options::options_description options::hidden;
boost::program_options::options_description options::all;
boost::program_options::positional_options_description options::positional;
boost::program_options::variables_map options::vm;

void options::get_options(int argc, char ** argv)
{
    visible.add_options()
        ("help", "show help message")
    ;
    hidden.add_options()
        ("filename", boost::program_options::value<std::string>(), "input file path")
    ;
    all.add(visible).add(hidden);
    positional.add("filename", 1);

    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(all).positional(positional).run(), vm);
    boost::program_options::notify(vm);
}
