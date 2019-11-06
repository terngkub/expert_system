#include <boost/spirit/home/x3.hpp>
#include <iostream>

namespace x3 = boost::spirit::x3;

x3::rule<class fact> const fact("fact");

x3::rule<class right_term> const right_term("right_term");
x3::rule<class right_expr> const right_expr("right_expr");

x3::rule<class left_term> const left_term("left_term");
x3::rule<class lv1> const lv1("lv1");
x3::rule<class lv2> const lv2("lv2");
x3::rule<class lv3> const lv3("lv3");
x3::rule<class left_expr> const left_expr("left_epxr");

x3::rule<class input> const input("input");

using x3::upper;
using x3::string;

auto const fact_def = upper;

auto const right_term_def = fact | '(' >> right_expr >> ')';
auto const right_expr_def = right_term >> *(string("+") >> right_term);

auto const left_term_def = fact | '(' >> left_expr >> ')';
auto const lv1_def = left_term | *(string("!")) >> left_term;
auto const lv2_def = lv1 >> *(string("+") >> lv1);
auto const lv3_def = lv2 >> *(string("|") >> lv2);
auto const left_expr_def = lv3 >> *(string("^") >> lv3);

auto const input_def = left_expr >> string("=>") >> right_expr;

BOOST_SPIRIT_DEFINE(fact);

BOOST_SPIRIT_DEFINE(right_term);
BOOST_SPIRIT_DEFINE(right_expr);

BOOST_SPIRIT_DEFINE(left_term);
BOOST_SPIRIT_DEFINE(lv1);
BOOST_SPIRIT_DEFINE(lv2);
BOOST_SPIRIT_DEFINE(lv3);
BOOST_SPIRIT_DEFINE(left_expr);

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