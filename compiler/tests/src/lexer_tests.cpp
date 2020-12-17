#define CATCH_CONFIG_RUNNER
#include <catch2/catch2.hpp>

#include <lexer.h>

TEST_CASE("Lexer lexes numbers correctly")
{
    compiler::Lexer lexer("2345,");
    auto result = lexer.Lex();
    REQUIRE(result.size() == 1);
}
