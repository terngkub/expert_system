#pragma once
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <list>

namespace ast
{

namespace x3 = boost::spirit::x3;

struct expr;
struct signed_;

struct operand : x3::variant<
    char
    , x3::forward_ast<signed_>
    , x3::forward_ast<expr>
>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct signed_
{
    char sign;
    operand operand_;
};

struct operation
{
    char operator_;
    operand operand_;
};

struct expr
{
    operand first;
    std::list<operation> rest;
};

struct rule
{
    expr left;
    std::string operator_;
    expr right;
};

}

BOOST_FUSION_ADAPT_STRUCT(ast::signed_, sign, operand_)
BOOST_FUSION_ADAPT_STRUCT(ast::operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(ast::expr, first, rest)
BOOST_FUSION_ADAPT_STRUCT(ast::rule, left, operator_, right)