#pragma once
#include <string>

// Reading
void handle_eof();

// Printing
const std::string hr{"------------------------------"};
std::string indent(int i);

// Overloaded Lambda
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
