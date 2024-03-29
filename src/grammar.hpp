#pragma once
#include "ast.hpp"
#include <vector>

namespace grammar
{

// rule

namespace x3 = boost::spirit::x3;

x3::rule<class right_term, ast::operand> const right_term("right_term");
x3::rule<class right_expr, ast::expr> const right_expr("right_expr");

x3::rule<class lv1, ast::operand> const lv1("lv1");
x3::rule<class lv2, ast::expr> const lv2("lv2");
x3::rule<class lv3, ast::expr> const lv3("lv3");
x3::rule<class left_expr, ast::expr> const left_expr("left_epxr");

x3::rule<class input, ast::rule> const input("input");

using x3::upper;
using x3::char_;
using x3::string;
using x3::omit;

auto const right_term_def = upper | '(' >> right_expr >> ')';
auto const right_expr_def = right_term >> *(char_("+") >> right_term);

auto const lv1_def = upper | '(' >> left_expr >> ')' | char_('!') >> lv1;
auto const lv2_def = lv1 >> *(char_("+") >> lv1);
auto const lv3_def = lv2 >> *(char_("|") >> lv2);
auto const left_expr_def = lv3 >> *(char_("^") >> lv3);

auto const input_def = left_expr >> string("=>") >> right_expr >> -('#' >> *omit[char_]);

BOOST_SPIRIT_DEFINE(right_term);
BOOST_SPIRIT_DEFINE(right_expr);
BOOST_SPIRIT_DEFINE(lv1);
BOOST_SPIRIT_DEFINE(lv2);
BOOST_SPIRIT_DEFINE(lv3);
BOOST_SPIRIT_DEFINE(left_expr);
BOOST_SPIRIT_DEFINE(input);


// initial_facts

x3::rule<class initial_facts, std::vector<char>> const initial_facts("initial_facts");
auto const initial_facts_def = '=' >> *(upper) >> -('#' >> *omit[char_]);
BOOST_SPIRIT_DEFINE(initial_facts);


// queries

x3::rule<class queries, std::vector<char>> const queries("queries");
auto const queries_def = '?' >> +(upper) >> -('#' >> *omit[char_]);
BOOST_SPIRIT_DEFINE(queries);

}