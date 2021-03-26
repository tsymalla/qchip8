#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>
#include <string_view>
#include <ostream>
#include <variant>

namespace compiler
{
    class Token final
    {
    public:
        using ValueType = std::variant<std::monostate, int, double, const char*>;
        struct NumberRange
        {
            size_t start;
            size_t end;
        };

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
            COMPARE             = 11,
            PLUS                = 12,
            MINUS               = 13,
            ASTERISK            = 14,
            SLASH               = 15,
            COMMA               = 16,
            SEMICOLON           = 17,
            COMMENT             = 18,
            KEYWORD             = 19,
            UNDEFINED			= 20
        };

        explicit Token(TokenKind kind, std::string_view content, size_t start, size_t end, const ValueType& value = std::monostate());

        static Token MakeToken(TokenKind kind, std::string_view content, size_t start, size_t end, const ValueType& value = std::monostate())
        {
            return Token{kind, content, start, end, value};
        }

        static const char* getNameFromTokenKind(const TokenKind tokenKind);
        TokenKind getKind() const;
        const char* getName() const;
        std::string getLexeme() const;
        ValueType getValue() const;
        NumberRange getRange() const;

        friend std::ostream& operator<<(std::ostream& rhs, const Token& token);
    private:
        TokenKind _kind;
        std::string _lexeme;
        ValueType _value;
        NumberRange _range;

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
            "COMPARE",
            "PLUS",
            "MINUS",
            "ASTERISK",
            "SLASH",
            "COMMA",
            "SEMICOLON",
            "COMMENT",
            "KEYWORD",
            "UNDEFINED"
        };
    };

    class Lexer final
    {
        constexpr static const char* KEYWORDS[] =
        {
            "program",
            "var",
            "const",
            "if",
            "else",
            "void",
            "number",
            "bool",
            "while",
            "return"
        };

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
        bool _isComparator(char c) const;
        bool _isKeyword(std::string_view identifier) const;
        bool _isDone() const;

        Token _getSlashOrComment();
        Token _getIdentifier();
        Token _getNumber();
        Token _getSpecialCharacter();
        Token _getArithmeticOperator();
        Token _getComparator();
        Token _getKeyword();

        Token _getNextToken();
    public:
        Lexer(std::string input);

        std::vector<Token> Lex();
    };
}

#endif
