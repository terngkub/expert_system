#pragma once
#include <boost/program_options.hpp>

struct options
{
    static boost::program_options::options_description hidden;
    static boost::program_options::options_description visible;
    static boost::program_options::options_description all;
    static boost::program_options::positional_options_description positional;
    static boost::program_options::variables_map vm;
    static void get_options(int argc, char ** argv);
};