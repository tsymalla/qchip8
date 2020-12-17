#include <lexer.h>

namespace compiler
{
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
        if (_currentPos >= _input.length())
        {
            return;
        }

        ++_currentPos;
    }

    char Lexer::_currentChar() const
    {
        return _input[_currentPos];
    }

    bool Lexer::_isDigit(char c) const
    {
        return std::isdigit(static_cast<int>(c));
    }

    bool Lexer::_isSpace(char c) const
    {
        return (c == ' ' || c == '\t');
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

        int number = std::stoi(_input.substr(start, end));

        return Token::TokenKind(Token::TokenKind::NUMBER);
    }

    Token Lexer::_getNextToken()
    {
        while (_isSpace(_currentChar()))
        {
            _forward();
        }

        if (_isDigit(_currentChar()))
        {
            return _getNumber();
        }

        return Token(Token::TokenKind::COMMA);
    }

    std::vector<Token> Lexer::Lex()
    {
        _tokens.clear();

        Token currentToken = _getNextToken();
        while (currentToken.getKind() != Token::TokenKind::COMMA)
        {
            _tokens.push_back(currentToken);
            currentToken = _getNextToken();
        }

        return _tokens;
    }

}
