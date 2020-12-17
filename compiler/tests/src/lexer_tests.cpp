#define CATCH_CONFIG_RUNNER
#include <catch2/catch2.hpp>

#include <lexer.h>
#include <iostream>

using compiler::Token;

static auto lexAndReturnNumberOfTokens(std::string input)
{
    std::cout << input << std::endl;
    compiler::Lexer lexer(std::move(input));
    auto result = lexer.Lex();

    for (const auto& token: result)
    {
        std::cout << token << " ";
    }

    std::cout << std::endl << "-------" << std::endl;
    
    return result;
}

TEST_CASE("Lexer lexes numbers correctly")
{
    auto tokens = lexAndReturnNumberOfTokens("23455,abcd");
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0].getKind() == Token::TokenKind::NUMBER);
    REQUIRE(tokens[1].getKind() == Token::TokenKind::COMMA);
    REQUIRE(tokens[2].getKind() == Token::TokenKind::ID);
}

TEST_CASE("Lexer lexes identifiers correctly")
{
    auto tokens = lexAndReturnNumberOfTokens("23455,abcd");
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0].getKind() == Token::TokenKind::NUMBER);
    REQUIRE(tokens[1].getKind() == Token::TokenKind::COMMA);
    REQUIRE(tokens[2].getKind() == Token::TokenKind::ID);
}

TEST_CASE("Lexer lexes spaces correctly")
{
    auto tokens = lexAndReturnNumberOfTokens("    23455    ,    abcd");
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0].getKind() == Token::TokenKind::NUMBER);
    REQUIRE(tokens[1].getKind() == Token::TokenKind::COMMA);
    REQUIRE(tokens[2].getKind() == Token::TokenKind::ID);
}

TEST_CASE("Lexer lexes braces correctly")
{
    auto tokens = lexAndReturnNumberOfTokens("((((){}))))");
    REQUIRE(tokens.size() == 11);
}

TEST_CASE("Lexer lexes arithmetic operators correctly")
{
    auto tokens = lexAndReturnNumberOfTokens("((2+3)/4)*5-8");
    REQUIRE(tokens.size() == 13);
    REQUIRE(tokens[3].getKind() == Token::TokenKind::PLUS);
    REQUIRE(tokens[6].getKind() == Token::TokenKind::SLASH);
    REQUIRE(tokens[9].getKind() == Token::TokenKind::ASTERISK);
    REQUIRE(tokens[11].getKind() == Token::TokenKind::MINUS);
}