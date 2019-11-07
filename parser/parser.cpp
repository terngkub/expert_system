#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <iostream>
#include <list>

namespace x3 = boost::spirit::x3;

namespace ast
{
    struct left_expr;
    struct right_expr;
    struct signed_;

    struct operand : x3::variant<
        char
        , x3::forward_ast<signed_>
        , x3::forward_ast<left_expr>
        , x3::forward_ast<right_expr>
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

    struct left_expr
    {
        operand first;
        std::list<operation> rest;
    };

    struct right_expr
    {
        operand first;
        std::list<operation> rest;
    };

    struct rule
    {
        left_expr left;
        std::string operator_;
        right_expr right;
    };
}

BOOST_FUSION_ADAPT_STRUCT(ast::signed_, sign, operand_)
BOOST_FUSION_ADAPT_STRUCT(ast::operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(ast::left_expr, first, rest)
BOOST_FUSION_ADAPT_STRUCT(ast::right_expr, first, rest)
BOOST_FUSION_ADAPT_STRUCT(ast::rule, left, operator_, right)


namespace grammar
{

x3::rule<class right_term, ast::operand> const right_term("right_term");
x3::rule<class right_expr, ast::right_expr> const right_expr("right_expr");

x3::rule<class lv1, ast::operand> const lv1("lv1");
x3::rule<class lv2, ast::left_expr> const lv2("lv2");
x3::rule<class lv3, ast::left_expr> const lv3("lv3");
x3::rule<class left_expr, ast::left_expr> const left_expr("left_epxr");

x3::rule<class input, ast::rule> const input("input");

using x3::upper;
using x3::char_;
using x3::string;

auto const right_term_def = upper | '(' >> right_expr >> ')';
auto const right_expr_def = right_term >> *(char_("+") >> right_term);

auto const lv1_def = upper | '(' >> left_expr >> ')' | char_('!') >> lv1;
auto const lv2_def = lv1 >> *(char_("+") >> lv1);
auto const lv3_def = lv2 >> *(char_("|") >> lv2);
auto const left_expr_def = lv3 >> *(char_("^") >> lv3);

auto const input_def = left_expr >> string("=>") >> right_expr;

BOOST_SPIRIT_DEFINE(right_term);
BOOST_SPIRIT_DEFINE(right_expr);
BOOST_SPIRIT_DEFINE(lv1);
BOOST_SPIRIT_DEFINE(lv2);
BOOST_SPIRIT_DEFINE(lv3);
BOOST_SPIRIT_DEFINE(left_expr);
BOOST_SPIRIT_DEFINE(input);

}

struct printer
{
    void operator()(ast::rule & rule)
    {
        (*this)(rule.left);
        std::cout << " => ";
        (*this)(rule.right);
        std::cout << "\n";
    }

    void operator()(ast::left_expr & expr)
    {
        boost::apply_visitor(*this, expr.first);
        for (auto & operation : expr.rest)
        {
            (*this)(operation);
        }
    }

    void operator()(ast::right_expr & expr)
    {
        boost::apply_visitor(*this, expr.first);
        for (auto & operation : expr.rest)
        {
            (*this)(operation);
        }
    }

    void operator()(ast::operation & operation)
    {
        std::cout << ' ' << operation.operator_ << ' ';
        (*this)(operation.operand_);
    }

    void operator()(ast::signed_ & signed_)
    {
        if (signed_.sign == '!')
            std::cout << "!(";
        (*this)(signed_.operand_);
        if (signed_.sign == '!')
            std::cout << ')';
    }

    void operator()(char c)
    {
        std::cout << c;
    }

    void operator()(ast::operand & operand)
    {
        boost::apply_visitor(*this, operand);
    }
};

int main()
{
    using boost::spirit::x3::ascii::space;

    std::string str;
    printer print;

    while (std::getline(std::cin, str))
    {
        ast::rule result;

        auto iter = str.cbegin();
        auto const end = str.cend();
        bool r = phrase_parse(iter, end, grammar::input, space, result);
        if (r && iter == end)
        {
            std::cout << "success\n";
            print(result);
        }
        else
        {
            std::cout << "fail\n";
        }
    }
}