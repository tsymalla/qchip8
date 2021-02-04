#define CATCH_CONFIG_RUNNER
#include <catch2/catch2.hpp>

#include <lexer.hpp>
#include <parser.hpp>
#include <iostream>

using compiler::Token;

static bool parseInput(std::string input)
{
    std::cout << input << std::endl;
    compiler::Lexer lexer(std::move(input));
    auto result = lexer.Lex();

    compiler::Parser parser(result);
    
    return parser.Parse();
}

TEST_CASE("Parser is able to parse a simple program without statements")
{
    CHECK(parseInput("program d123 { }"));
}

TEST_CASE("Parser is able to determine a simple invalid program")
{
    CHECK(!parseInput("program d123"));
}