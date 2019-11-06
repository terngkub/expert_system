#include <boost/spirit/home/x3.hpp>
#include <iostream>

namespace x3 = boost::spirit::x3;

x3::rule<class fact> const fact("fact");
x3::rule<class right_term> const right_term("right_term");
x3::rule<class right_expr> const right_expr("right_expr");
x3::rule<class input> const input("input");

using x3::upper;
using x3::string;

auto const fact_def = upper;
auto const right_term_def = fact | '(' >> right_expr >> ')';
auto const right_expr_def = right_term >> *(string("+") >> right_term);
auto const input_def = right_expr;

BOOST_SPIRIT_DEFINE(fact);
BOOST_SPIRIT_DEFINE(right_term);
BOOST_SPIRIT_DEFINE(right_expr);
BOOST_SPIRIT_DEFINE(input);

auto grammar = input;

int main()
{
    using boost::spirit::x3::ascii::space;

    std::string str;
    while (std::getline(std::cin, str))
    {
        auto iter = str.cbegin();
        auto const end = str.cend();
        bool r = phrase_parse(iter, end, grammar, space);
        if (r && iter == end)
        {
            std::cout << "success\n";
        }
        else
        {
            std::cout << "fail\n";
        }
    }
}