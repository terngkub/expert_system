#pragma once
#include "fact.hpp"
#include "rule.hpp"
#include "parser.hpp"
#include <fstream>
#include <map>
#include <memory>
#include <vector>

struct expert_system
{
    std::map<char, std::shared_ptr<fact>> facts;
    std::vector<std::shared_ptr<rule>> rules;
    std::vector<char> queries;
    parser parser_;

    expert_system(std::string & filename);

    void operator()();
    void run();
    void query(std::shared_ptr<fact> f);
    void print();
    
    void debug_print();

    void interactive_loop();
    void interactive_reset();
    void interactive_exit(std::string str);
    void interactive_initial_facts();
    void interactive_query();

};
