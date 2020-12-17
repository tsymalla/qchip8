#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <ostream>

namespace compiler
{
    class Token final
    {
    public:
        enum class TokenKind : size_t
        {
            NUMBER              = 1,
            ID                  = 2,
            OPEN_PARENTHESIS    = 3,
            CLOSE_PARENTHESIS   = 4,
            OPEN_CURLY_BRACKET  = 5,
            CLOSE_CURLY_BRACKET = 6,
            LESS                = 7,
            LESS_OR_EQUAL       = 8,
            GREATER             = 9,
            GREATER_OR_EQUAL    = 10,
            EQUAL               = 11,
            PLUS                = 12,
            MINUS               = 13,
            ASTERISK            = 14,
            SLASH               = 15,
            COMMA               = 16,
            SEMICOLON           = 17,
            COMMENT             = 18
        };

        Token(TokenKind kind);

        TokenKind getKind() const;
        const char* getName() const;

        std::ostream& operator<<(std::ostream& rhs)
        {
            rhs << getName();
            return rhs;
        }

    private:
        TokenKind _kind;

        constexpr static const char* TOKEN_NAMES[] =
        {
            "NUMBER",
            "ID",
            "(",
            ")",
            "{",
            "}",
            "<",
            "<=",
            ">",
            ">=",
            "=",
            "+",
            "-",
            "*",
            "/",
            ","
            ";",
            "//"
        };
    };

    class Lexer final
    {
        std::string _input;
        std::vector<Token> _tokens;
        long _currentPos;

        void _forward();
        char _currentChar() const;
        bool _isDigit(char c) const;
        bool _isSpace(char c) const;
        bool _isDone() const;

        Token _getNumber();

        Token _getNextToken();
    public:
        Lexer(std::string input);

        std::vector<Token> Lex();
    };
}

#endif
