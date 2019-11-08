#pragma once
#include "ast.hpp"
#include <iostream>

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