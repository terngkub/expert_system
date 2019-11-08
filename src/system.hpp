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
    parser parser_;

    expert_system(std::string & filename);

    void operator()();

    fact_value query(std::shared_ptr<fact> f);



    /* for debug*/
    expert_system();
};
