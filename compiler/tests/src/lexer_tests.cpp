#define CATCH_CONFIG_RUNNER
#include <catch2/catch2.hpp>

#include <lexer.h>

static int lexAndReturnNumberOfTokens(std::string input)
{
    compiler::Lexer lexer(std::move(input));
    auto result = lexer.Lex();
    
    return result.size();
}

TEST_CASE("Lexer lexes numbers correctly")
{
    auto tokenCount = lexAndReturnNumberOfTokens("23455,");
    REQUIRE(tokenCount == 2);
}

TEST_CASE("Lexer lexes spaces correctly")
{
    auto tokenCount = lexAndReturnNumberOfTokens("    23455    ,");
    REQUIRE(tokenCount == 2);
}
