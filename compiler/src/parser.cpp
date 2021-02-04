#include <parser.hpp>
#include <iostream>

namespace compiler
{
	Parser::Parser(const std::vector<Token>& tokens) : _tokens{ tokens }, _tokenIndex { -1 }
	{
		if (!_tokens.empty())
		{
			_tokenIndex = 0;
		}
	}

	bool Parser::Parse()
	{
	    _parseProgram();

	    if (!_errors.empty())
        {
	        std::cout << "Gathered " << _errors.size() << " parsing errors:" << std::endl;
	        for (const auto& err: _errors)
            {
	            std::cout << err << std::endl;
            }

	        return false;
        }

	    return true;
	}

    bool Parser::_hasToken() const
    {
	    return _tokenIndex < _tokens.size();
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
	    if (!_hasToken())
        {
            _handleEndOfInput(tokenKind);
	        return;
        }

        const Token& token = _getCurrentToken();
        if (token.getKind() != tokenKind)
        {
            _handleError(token, "");
        }

        ++_tokenIndex;
    }

    void Parser::_match(Token::TokenKind tokenKind, std::string_view lexeme)
	{
        if (!_hasToken())
        {
            _handleEndOfInput(tokenKind, lexeme);
            return;
        }

	    const Token& token = _getCurrentToken();
		if (token.getKind() != tokenKind && token.getLexeme() != lexeme)
		{
            _handleError(token, lexeme);
		}

        ++_tokenIndex;
	}

    void Parser::_parseProgram()
    {
	    _match(Token::TokenKind::KEYWORD, "program");
	    _match(Token::TokenKind::ID);
	    _match(Token::TokenKind::OPEN_CURLY_BRACKET);
	    _parseStatements();
        _match(Token::TokenKind::CLOSE_CURLY_BRACKET);
    }

    void Parser::_parseStatements()
    {

    }

	void Parser::_parseExpression()
	{
	}

	void Parser::_parseConstant()
	{
	}

	void Parser::_parseStringLiteral()
	{
	}

	void Parser::_handleError(Token token, std::string_view lexeme)
	{
	    if (lexeme == "")
        {
            _errors.push_back(std::string("Parse error: Expected ") + token.getLexeme());
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