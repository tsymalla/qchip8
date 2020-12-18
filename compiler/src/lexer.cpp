#include <lexer.h>

namespace compiler
{
    std::ostream& operator<<(std::ostream& rhs, const Token& token)
    {
        rhs << token.getName();
        return rhs;
    }

    Token::Token(Token::TokenKind kind, size_t start, size_t end) :
        _kind{ kind },
        _range{ start, end }
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

    Token::NumberRange Token::getRange() const
    {
        return _range;
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
        return (c == ' ' || c == '\t' || c == '\n');
    }

    bool Lexer::_isSpecialCharacter(char c) const
    {
        return
        (
            c == '(' ||
            c == ')' ||
            c == '{' ||
            c == '}' ||
            c == ';' ||
            c == '='
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

    bool Lexer::_isComparator(char c) const
    {
        return
            (
                c == '>' ||
                c == '<'
            );
    }

    bool Lexer::_isKeyword(std::string_view identifier) const
    {
        return std::find(std::begin(KEYWORDS), std::end(KEYWORDS), identifier) != std::end(KEYWORDS);
    }

    bool Lexer::_isDone() const
    {
        return _currentPos >= _input.length();
    }

    Token Lexer::_getSlashOrComment()
    {
        auto start = _currentPos;
        int end = start;

        if (_currentChar() == '/')
        {
            _forward();

            while (_currentChar() != '\n' && _currentChar() != '\0')
            {
                _forward();
                ++end;
            }

            return Token(Token::TokenKind::COMMENT, start, end);
        }

        // move backwards
        _currentPos = start;
        return Token(Token::TokenKind::SLASH, start - 1, end);
    }

    Token Lexer::_getIdentifier()
    {
        auto start = _currentPos;
        int end = start;

        _forward();

        while (!_isSpace(_currentChar()) && (_isDigit(_currentChar()) || _isLetter(_currentChar())))
        {
            ++end;
            _forward();
        }

        std::string identifier = _input.substr(start, (end - start) + 1);

        if (_isKeyword(identifier))
        {
            return Token(Token::TokenKind::KEYWORD, start, end + 1);
        }

        return Token(Token::TokenKind::ID, start, end + 1);
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

        return Token(Token::TokenKind::NUMBER, start, end);
    }

    Token Lexer::_getKeyword()
    {
        return Token(Token::TokenKind::GREATER, 0, 0);
    }

    Token Lexer::_getSpecialCharacter()
    {
        auto start = _currentPos;
        char currentChar = _currentChar();
        _forward();

        Token::TokenKind kind;

        if (currentChar == '(')
        {
            kind = Token::TokenKind::OPEN_PARENTHESIS;
        }
        else if (currentChar == ')')
        {
            kind = Token::TokenKind::CLOSE_PARENTHESIS;
        }
        else if (currentChar == '{')
        {
            kind = Token::TokenKind::OPEN_CURLY_BRACKET;
        }
        else if (currentChar == '}')
        {
            kind = Token::TokenKind::CLOSE_CURLY_BRACKET;
        }
        else if (currentChar == '=')
        {
            kind = Token::TokenKind::EQUAL;
        }
        else
        {
            kind = Token::TokenKind::SEMICOLON;
        }

        return Token(kind, start, start + 1);
    }

    Token Lexer::_getArithmeticOperator()
    {
        auto start = _currentPos;
        char currentChar = _currentChar();
        _forward();

        if (currentChar == '+')
        {
            return Token(Token::TokenKind::PLUS, start, start + 1);
        }
        else if (currentChar == '-')
        {
            return Token(Token::TokenKind::MINUS, start, start + 1);
        }
        else if (currentChar == '*')
        {
            return Token(Token::TokenKind::ASTERISK, start, start + 1);
        }

        return _getSlashOrComment();
    }

    Token Lexer::_getComparator()
    {
        auto start = _currentPos;
        char currentChar = _currentChar();
        _forward();

        Token::TokenKind kind;
        if (currentChar == '+')
        {
            kind = Token::TokenKind::GREATER;
        }
        else if (currentChar == '-')
        {
            kind = Token::TokenKind::LESS;
        }
        else if (currentChar == '*')
        {
            kind = Token::TokenKind::EQUAL;
        }

        return Token(kind, start, start + 1);
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

        if (_isComparator(_currentChar()))
        {
            return _getComparator();
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
        return Token(Token::TokenKind::COMMA, _currentPos, _currentPos + 1);
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
