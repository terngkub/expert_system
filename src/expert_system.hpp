#pragma once
#include "fact.hpp"
#include "rule.hpp"
#include "parser.hpp"
#include <fstream>
#include <map>
#include <memory>
#include <vector>

class expert_system
{
public:
    // Public Attributes
    std::map<char, std::shared_ptr<fact>> facts;
    std::vector<std::shared_ptr<rule>> rules;
    std::vector<char> queries;

    // Constructor and Destructor
    expert_system() = delete;
    expert_system(std::string & filename);
    ~expert_system() = default;

    // Copy - disable
    expert_system(expert_system const &) = delete;
    expert_system & operator=(expert_system const &) = delete;

    // Move - disable
    expert_system(expert_system &&) = delete;
    expert_system & operator=(expert_system &&) = delete;

    // Function Object
    void operator()();

private:
    // Private Attributes
    parser parser_;

    // Internal
    void run();
    void query(std::shared_ptr<fact> f);
    void print();
    
    // Debug
    void debug_print();

    // Interactive
    void interactive_loop();
    void interactive_reset();
    void interactive_exit(std::string str);
    void interactive_initial_facts();
    void interactive_query();
};
