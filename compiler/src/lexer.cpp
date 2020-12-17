#include <lexer.h>

namespace compiler
{
    std::ostream& operator<<(std::ostream& rhs, const Token& token)
    {
        rhs << token.getName();
        return rhs;
    }

    Token::Token(Token::TokenKind kind) :
        _kind{kind}
    {
    }

    Token::TokenKind Token::getKind() const
    {
        return _kind;
    }

    const char* Token::getName() const
    {
        return TOKEN_NAMES[static_cast<size_t>(_kind)];
    }

    Lexer::Lexer(std::string input):
        _input{std::move(input)},
        _currentPos{0}
    {
    }

    void Lexer::_forward()
    {
        ++_currentPos;
    }

    char Lexer::_currentChar() const
    {
        return _input[_currentPos];
    }

    bool Lexer::_isLetter(char c) const
    {
        return std::isalpha(static_cast<int>(c));
    }

    bool Lexer::_isDigit(char c) const
    {
        return std::isdigit(static_cast<int>(c));
    }

    bool Lexer::_isSpace(char c) const
    {
        return (c == ' ' || c == '\t');
    }

    bool Lexer::_isSpecialCharacter(char c) const
    {
        return
        (
            c == '(' ||
            c == ')' ||
            c == '{' ||
            c == '}'
        );
    }

    bool Lexer::_isArithmeticOperator(char c) const
    {
        return
        (
            c == '+' ||
            c == '-' ||
            c == '*' ||
            c == '/'
        );
    }

    bool Lexer::_isDone() const
    {
        return _currentPos >= _input.length();
    }

    Token Lexer::_getIdentifier()
    {
        auto start = _currentPos;
        int end = start;

        _forward();

        while (_isDigit(_currentChar()) || _isLetter(_currentChar()))
        {
            ++end;
            _forward();
        }

        std::string identifier = _input.substr(start, end + 1);

        return Token(Token::TokenKind::ID);
    }

    Token Lexer::_getNumber()
    {
        auto start = _currentPos;
        int end = start;

        _forward();

        while (_isDigit(_currentChar()))
        {
            ++end;
            _forward();
        }

        int number = std::stoi(_input.substr(start, end + 1));

        return Token(Token::TokenKind::NUMBER);
    }

    Token Lexer::_getKeyword()
    {
        return Token(Token::TokenKind::GREATER);
    }

    Token Lexer::_getSpecialCharacter()
    {
        char currentChar = _currentChar();
        _forward();

        if (currentChar == '(')
        {
            return Token(Token::TokenKind::OPEN_PARENTHESIS);
        }
        else if (currentChar == ')')
        {
            return Token(Token::TokenKind::CLOSE_PARENTHESIS);
        }
        else if (currentChar == '{')
        {
            return Token(Token::TokenKind::OPEN_CURLY_BRACKET);
        }
        
        return Token(Token::TokenKind::CLOSE_CURLY_BRACKET);
    }

    Token Lexer::_getArithmeticOperator()
    {
        char currentChar = _currentChar();
        _forward();

        if (currentChar == '+')
        {
            return Token(Token::TokenKind::PLUS);
        }
        else if (currentChar == '-')
        {
            return Token(Token::TokenKind::MINUS);
        }
        else if (currentChar == '*')
        {
            return Token(Token::TokenKind::ASTERISK);
        }

        return Token(Token::TokenKind::SLASH);
    }

    Token Lexer::_getNextToken()
    {
        while (_isSpace(_currentChar()))
        {
            _forward();
        }

        if (_isSpecialCharacter(_currentChar()))
        {
            return _getSpecialCharacter();
        }

        if (_isArithmeticOperator(_currentChar()))
        {
            return _getArithmeticOperator();
        }

        if (_isLetter(_currentChar()))
        {
            return _getIdentifier();
        }

        if (_isDigit(_currentChar()))
        {
            return _getNumber();
        }

        _forward();
        return Token(Token::TokenKind::COMMA);
    }

    std::vector<Token> Lexer::Lex()
    {
        _tokens.clear();

        while (!_isDone())
        {
            auto currentToken = _getNextToken();
            _tokens.push_back(currentToken);
        }

        return _tokens;
    }
}
