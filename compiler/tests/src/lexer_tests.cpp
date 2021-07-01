#define CATCH_CONFIG_RUNNER
#include <catch2/catch2.hpp>

#include <lexer.hpp>
#include <iostream>

using compiler::Token;

static auto lexAndReturnNumberOfTokens(std::string input)
{
    std::cout << input << std::endl;
    compiler::Lexer lexer(std::move(input));
    auto result = lexer.Lex();

    std::cout << std::endl << "Tokens:" << std::endl;
    for (const auto& token: result)
    {
        std::cout << token << "\t";
    }

    std::cout << std::endl;
    
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

TEST_CASE("Lexer lexes comments correctly")
{
    auto tokens = lexAndReturnNumberOfTokens("1+2//comment");
    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0].getKind() == Token::TokenKind::NUMBER);
    REQUIRE(tokens[1].getKind() == Token::TokenKind::PLUS);
    REQUIRE(tokens[2].getKind() == Token::TokenKind::NUMBER);
    REQUIRE(tokens[3].getKind() == Token::TokenKind::COMMENT);
}

TEST_CASE("Lexer lexes multi-lines correctly")
{
    auto tokens = lexAndReturnNumberOfTokens(
        "1+2; // comment\n \
        3+4");
    REQUIRE(tokens.size() == 8);
    REQUIRE(tokens[0].getKind() == Token::TokenKind::NUMBER);
    REQUIRE(tokens[1].getKind() == Token::TokenKind::PLUS);
    REQUIRE(tokens[2].getKind() == Token::TokenKind::NUMBER);
    REQUIRE(tokens[3].getKind() == Token::TokenKind::SEMICOLON);
    REQUIRE(tokens[4].getKind() == Token::TokenKind::COMMENT);
    REQUIRE(tokens[5].getKind() == Token::TokenKind::NUMBER);
    REQUIRE(tokens[6].getKind() == Token::TokenKind::PLUS);
    REQUIRE(tokens[7].getKind() == Token::TokenKind::NUMBER);
}

TEST_CASE("Lexer lexes keywords correctly")
{
    auto tokens = lexAndReturnNumberOfTokens(
        "var i=5; // variable\n \
        const j=3;");

    REQUIRE(tokens.size() == 11);
    REQUIRE(tokens[0].getKind() == Token::TokenKind::KEYWORD);
    REQUIRE(tokens[1].getKind() == Token::TokenKind::ID);
    REQUIRE(tokens[2].getKind() == Token::TokenKind::EQUAL);
    REQUIRE(tokens[3].getKind() == Token::TokenKind::NUMBER);
    REQUIRE(tokens[4].getKind() == Token::TokenKind::SEMICOLON);
    REQUIRE(tokens[5].getKind() == Token::TokenKind::COMMENT);
    REQUIRE(tokens[6].getKind() == Token::TokenKind::KEYWORD);
    REQUIRE(tokens[7].getKind() == Token::TokenKind::ID);
    REQUIRE(tokens[8].getKind() == Token::TokenKind::EQUAL);
    REQUIRE(tokens[9].getKind() == Token::TokenKind::NUMBER);
    REQUIRE(tokens[10].getKind() == Token::TokenKind::SEMICOLON);
}

TEST_CASE("Lexer lexes identifiers correctly")
{
    auto tokens = lexAndReturnNumberOfTokens((
            "var i123=5;"
    ));

    REQUIRE(tokens.size() == 5);
    REQUIRE(tokens[0].getKind() == Token::TokenKind::KEYWORD);
    REQUIRE(tokens[1].getKind() == Token::TokenKind::ID);
    REQUIRE(tokens[2].getKind() == Token::TokenKind::EQUAL);
    REQUIRE(tokens[3].getKind() == Token::TokenKind::NUMBER);
    REQUIRE(tokens[4].getKind() == Token::TokenKind::SEMICOLON);
}