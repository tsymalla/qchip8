#include <parser.hpp>
#include <iostream>

namespace compiler
{
    Parser::Parser(const std::vector<Token>& tokens) : _tokenIndex { -1 }, _tokens{ tokens }
	{
		if (!_tokens.empty())
		{
			_tokenIndex = 0;
		}
	}

	ExprNodePtr Parser::Parse()
	{
		if (!_parseProgram())
		{
			if (!_errors.empty())
			{
				std::cout << "Gathered " << _errors.size() << " parsing errors:" << std::endl;
				for (const auto& err: _errors)
				{
					std::cout << err << std::endl;
				}

				return nullptr;
			}
		}

		return std::make_unique<VariableNode>("hi");
	}

	bool Parser::_hasToken() const
	{
		return _tokenIndex < _tokens.size();
	}

	Token Parser::_getCurrentToken() const
	{
		if (_tokenIndex > -1 && _tokenIndex < _tokens.size())
		{
			return _tokens[_tokenIndex];
		}

		return NullToken;
	}

	bool Parser::_match(Token::TokenKind tokenKind)
	{
		if (!_hasToken())
		{
			_handleEndOfInput(tokenKind);
			return false;
		}

		const Token& token = _getCurrentToken();
		if (token.getKind() != tokenKind)
		{
			_handleError(token, tokenKind, "");
			return false;
		}

		++_tokenIndex;

		return true;
	}

	bool Parser::_match(Token::TokenKind tokenKind, std::string_view lexeme)
	{
		if (!_hasToken())
		{
			_handleEndOfInput(tokenKind, lexeme);
			return false;
		}

		const Token& token = _getCurrentToken();
		if (token.getKind() != tokenKind && token.getLexeme() != lexeme)
		{
			_handleError(token, tokenKind, lexeme);
			return false;
		}

		++_tokenIndex;

		return true;
	}

	bool Parser::_parseProgram()
	{
		return  _match(Token::TokenKind::KEYWORD, "program") &&
				_match(Token::TokenKind::ID) &&
				_parseBlock();
	}

	bool Parser::_parseBlock()
	{
		return  _match(Token::TokenKind::OPEN_CURLY_BRACKET) &&
				_parseStatements() &&
				_match(Token::TokenKind::CLOSE_CURLY_BRACKET);
	}

	bool Parser::_parseStatements()
	{
		if (_parseSingleStatement())
		{
			if (_parseStatements())
			{
				return true;
			}

			return false;
		}

		return _parseEmptyBlock();
	}

	bool Parser::_parseSingleStatement()
	{
		const auto& token = _getCurrentToken();
		if (token.getKind() == Token::TokenKind::ID)
		{
			return _parseAssignment();
		}
		else if (token.getKind() == Token::TokenKind::KEYWORD)
		{
			if (token.getLexeme() == "if")
			{
				return _parseConditional();
			}
			else if (token.getLexeme() == "while")
			{
				return _parseLoop();
			}
			else if (token.getLexeme() == "call")
			{
				// TODO
				return _parseFunctionCall();
			}
		}

		return false;
	}

	bool Parser::_parseAssignment()
	{
		// TODO use value here.
		return  _match(Token::TokenKind::ID) &&
				_match(Token::TokenKind::EQUAL) &&
                _match(Token::TokenKind::NUMBER) &&
				_match(Token::TokenKind::SEMICOLON);
	}

	bool Parser::_parseConditional()
	{
		return false;
	}

	bool Parser::_parseLoop()
	{
		return  _match(Token::TokenKind::KEYWORD, "while") &&
				_match(Token::TokenKind::OPEN_PARENTHESIS) &&
				_match(Token::TokenKind::ID) &&
				_match(Token::TokenKind::COMPARE) &&
				_match(Token::TokenKind::NUMBER) &&
				_match(Token::TokenKind::CLOSE_PARENTHESIS) &&
				_parseBlock();
	}

	bool Parser::_parseFunctionCall()
	{
		return false;
	}

	bool Parser::_parseEmptyBlock() const
	{
		return true;
	}

	void Parser::_handleError(Token token, Token::TokenKind tokenKind, std::string_view lexeme)
	{
		if (lexeme == "")
		{
			_errors.push_back(std::string("Parse error: Expected ") + Token::getNameFromTokenKind(tokenKind));
			return;
		}

		_errors.push_back(std::string("Parse error: Got ") + token.getLexeme() + ", expected: " + std::string(lexeme));
	}

	void Parser::_handleEndOfInput(Token::TokenKind tokenKind, std::string_view lexeme)
	{
		if (lexeme == "")
		{
			_errors.push_back(std::string("Determined end of input: Expected ") + Token::getNameFromTokenKind(tokenKind));
			return;
		}

		_errors.push_back(std::string("Determined end of input, expected ") + std::string(lexeme));
	}
}
