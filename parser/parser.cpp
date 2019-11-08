#include "ast.hpp"
#include "grammar.hpp"
#include "printer.hpp"

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