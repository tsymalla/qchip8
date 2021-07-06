#include <algorithm>
#include <lexer.hpp>
#include <sstream>

namespace compiler
{
	std::ostream& operator<<(std::ostream& rhs, const Token& token)
	{
		rhs << token.getName();
		return rhs;
	}

	Token::Token(Token::TokenKind kind, std::string_view content, size_t start, size_t end, const ValueType& value)
		: _kind{kind}, _value{value}, _range{start, end}
	{
		_lexeme = content.substr(start, end - start);
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

	std::string Token::getLexeme() const
	{
		return _lexeme;
	}

	Token::ValueType Token::getValue() const
	{
		return _value;
	}

	const char* Token::getNameFromTokenKind(const Token::TokenKind kind)
	{
		return TOKEN_NAMES[static_cast<size_t>(kind)];
	}

	bool Token::isComparisonOperator() const
	{
		return _kind >= Token::TokenKind::LESS && _kind <= Token::TokenKind::COMPARE;
	}

	Lexer::Lexer(std::string input) : _input{std::move(input)}, _currentPos{0}
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
		return (c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == '=');
	}

	bool Lexer::_isArithmeticOperator(char c) const
	{
		return (c == '+' || c == '-' || c == '*' || c == '/');
	}

	bool Lexer::_isComparator(char c) const
	{
		return (c == '>' || c == '<');
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

			return Token::MakeToken(Token::TokenKind::COMMENT, _input, start, end);
		}

		// move backwards
		_currentPos = start;
		return Token::MakeToken(Token::TokenKind::SLASH, _input, start - 1, end);
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

		Token::TokenKind kind = Token::TokenKind::ID;
		if (_isKeyword(identifier))
		{
			kind = Token::TokenKind::KEYWORD;
		}

		return Token::MakeToken(kind, _input, start, end + 1, identifier.c_str());
	}

	Token Lexer::_getNumber()
	{
		auto start = _currentPos;
		int end = start;
		std::stringstream numValue;
		numValue << _currentChar();

		_forward();

		while (_isDigit(_currentChar()))
		{
			++end;
			_forward();
		}

		return Token::MakeToken(Token::TokenKind::NUMBER, _input, start, end, std::stoi(numValue.str()));
	}

	Token Lexer::_getKeyword()
	{
		return Token::MakeToken(Token::TokenKind::GREATER, _input, 0, 0);
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

		return Token::MakeToken(kind, _input, start, start + 1, currentChar);
	}

	Token Lexer::_getArithmeticOperator()
	{
		auto start = _currentPos;
		char currentChar = _currentChar();
		_forward();

		if (currentChar == '+')
		{
			return Token::MakeToken(Token::TokenKind::PLUS, _input, start, start + 1, currentChar);
		}
		else if (currentChar == '-')
		{
			return Token::MakeToken(Token::TokenKind::MINUS, _input, start, start + 1, currentChar);
		}
		else if (currentChar == '*')
		{
			return Token::MakeToken(Token::TokenKind::ASTERISK, _input, start, start + 1, currentChar);
		}

		return _getSlashOrComment();
	}

	Token Lexer::_getComparator()
	{
		auto start = _currentPos;
		char currentChar = _currentChar();
		_forward();

		Token::TokenKind kind;
		if (currentChar == '<' || currentChar == '>')
		{
			kind = Token::TokenKind::COMPARE;
		}

		return Token::MakeToken(kind, _input, start, start + 1, currentChar);
	}

	Token Lexer::_getNextToken()
	{
		while (_isSpace(_currentChar()) && _currentPos < _input.length())
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

		if (_currentPos >= _input.length())
		{
			return Token::MakeToken(Token::TokenKind::END_OF_INPUT, _input);
		}

		return Token::MakeToken(Token::TokenKind::COMMA, _input, _currentPos, _currentPos + 1);
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
} // namespace compiler
