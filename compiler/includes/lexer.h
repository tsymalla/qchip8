#ifndef LEXER_H
#define LEXER_H

namespace compiler
{
    class Token final
    {
        enum class TokenKind: uint8_t
        {
            NUMBER,
            ID,
            OPEN_PARENTHESIS,
            CLOSE_PARENTHESIS,
            OPEN_CURLY_BRACKET,
            CLOSE_CURLY_BRACKET,
            LESS,
            LESS_OR_EQUAL,
            GREATER,
            GREATER_OR_EQUAL,
            EQUAL,
            PLUS,
            MINUS,
            ASTERISK,
            SLASH,
            COMMA,
            SEMICOLON,
            COMMENT
        };
    };

    class Lexer final
    {

    };
}

#endif
