#include <iostream>

int main()
{
    std::cout << "Hello, world!\n";
}

/*
class system
{
    std::map<char, fact> facts;
    std::vector<rule> rules;

    void operator()()
    {
        run();
    }
};

enum class fact_value
{
    true,
    false,
    undetermined
};

class fact
{
    fact_value value;
    std::vector<rule *> rules;
};

enum rule_operation
{
    not,
    and,
    or,
    xor,
    imply,
    if_and_only_if
};

class rule
{
    rule_operation operation;
    // fact can change value so no const
    // rule can't be change so const *
    std::variant<fact *, rule const *> left;
    std::variant<fact *, rule const *> right;

    fact_value evaluate()
    {
        auto l_value = (left is fact) ? left.value : left.evaluate();
        auto r_value = (right is fact) ? right.value : right.evaluate();
        return evaluate base on operation_t;
    }
};

*/