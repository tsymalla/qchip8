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
            NUMBER              = 0,
            ID                  = 1,
            OPEN_PARENTHESIS    = 2,
            CLOSE_PARENTHESIS   = 3,
            OPEN_CURLY_BRACKET  = 4,
            CLOSE_CURLY_BRACKET = 5,
            LESS                = 6,
            LESS_OR_EQUAL       = 7,
            GREATER             = 8,
            GREATER_OR_EQUAL    = 9,
            EQUAL               = 10,
            PLUS                = 11,
            MINUS               = 12,
            ASTERISK            = 13,
            SLASH               = 14,
            COMMA               = 15,
            SEMICOLON           = 16,
            COMMENT             = 17
        };

        Token(TokenKind kind);

        TokenKind getKind() const;
        const char* getName() const;

        friend std::ostream& operator<<(std::ostream& rhs, const Token& token);
    private:
        TokenKind _kind;

        constexpr static const char* TOKEN_NAMES[] =
        {
            "NUMBER",
            "ID",
            "OPEN_BRACKET",
            "CLOSE_BRACKET",
            "OPEN_CURLY_BRACKET",
            "CLOSE_CURLY_BRACKET",
            "LESS",
            "LESS_OR_EQUAL",
            "GREATER",
            "GREATER_OR_EQUAL",
            "EQUAL",
            "PLUS",
            "MINUS",
            "ASTERISK",
            "SLASH",
            "COMMA",
            "SEMICOLON",
            "COMMENT"
        };
    };

    class Lexer final
    {
        std::string _input;
        std::vector<Token> _tokens;
        long _currentPos;

        void _forward();
        char _currentChar() const;
        bool _isLetter(char c) const;
        bool _isDigit(char c) const;
        bool _isSpace(char c) const;
        bool _isSpecialCharacter(char c) const;
        bool _isArithmeticOperator(char c) const;
        bool _isDone() const;

        Token _getSlashOrComment();
        Token _getIdentifier();
        Token _getNumber();
        Token _getKeyword();
        Token _getSpecialCharacter();
        Token _getArithmeticOperator();

        Token _getNextToken();
    public:
        Lexer(std::string input);

        std::vector<Token> Lex();
    };
}

#endif
