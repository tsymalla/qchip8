#include <parser.hpp>

namespace compiler
{
	Parser::Parser(const std::vector<Token>& tokens) : _tokens{ tokens }, _tokenIndex { -1 }
	{
		if (!_tokens.empty())
		{
			_tokenIndex = 0;
		}
	}

	void Parser::Parse()
	{
	}

	Token Parser::_getCurrentToken() const
	{
		// TODO: How to handle out of range exceptions?
		if (_tokenIndex < _tokens.size())
		{
			return _tokens[_tokenIndex];
		}
	}

	void Parser::_match(Token::TokenKind tokenKind)
	{
		if (_getCurrentToken().getKind() == tokenKind)
		{
			++_tokenIndex;
			return;
		}

		_handleError(_getCurrentToken());
	}

	// TODO: Implement recursive descent parser.
	void Parser::_parseExpression()
	{
	}

	void Parser::_parseConstant()
	{
	}

	void Parser::_parseStringLiteral()
	{
	}

	void Parser::_handleError(Token token)
	{
		_errors.push_back(std::string("Parse error: ") + token.getName());
	}
}