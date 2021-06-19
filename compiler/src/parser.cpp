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

    ParserState const* Parser::Parse()
	{
        auto root = _parseProgram();
		if (!root)
		{
            if (_state.HasError())
			{
                std::cout << "Compilation error: " << _state.GetErrorMessages().size() << " errors:" << std::endl;
                for (const auto& error: _state.GetErrorMessages())
				{
                    std::cout << error << std::endl;
				}
			}
		}

        _state.SetAstRoot(std::move(root));

        return &_state;
	}

	bool Parser::_hasToken() const
	{
        return _tokenIndex < static_cast<int>(_tokens.size());
	}

	Token Parser::_getCurrentToken() const
	{
        if (_tokenIndex > -1 && _hasToken())
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

    Token Parser::_peek() const
    {
        if (!_hasToken())
        {
            return NullToken;
        }

        return _getCurrentToken();
    }

    void Parser::_advance()
    {
        ++_tokenIndex;
    }

    Token Parser::_expect(Token::TokenKind tokenKind) {
        Token nextToken = _peek();

        if (nextToken.getKind() == tokenKind)
        {
            return nextToken;
        }

        _handleError(nextToken, tokenKind);
        return NullToken;
    }

    NodePtr Parser::_parseID()
	{
		Token nextToken = _peek();
		if (nextToken.getKind() == Token::TokenKind::ID)
		{
			_advance();
			return std::make_unique<StringLiteralNode>(nextToken.getLexeme());
		}

		return nullptr;
	}

    NodePtr Parser::_parseProgram()
	{
        if (_match(Token::TokenKind::KEYWORD, "program"))
        {
            auto id = _parseID();
            auto root = std::make_unique<ProgramNode>(std::move(id));
            auto block = _parseBlock();
            root->AddBlock(std::move(block));


            return root;
        }

        return nullptr;
	}

    std::unique_ptr<BlockNode> Parser::_parseBlock()
	{
        if (_match(Token::TokenKind::OPEN_CURLY_BRACKET))
        {
            auto statements = _parseStatements();
            if (_match(Token::TokenKind::CLOSE_CURLY_BRACKET))
            {
                return std::make_unique<BlockNode>(statements);
            }
        }

        return nullptr;
	}

    std::vector<StatementNodePtr> Parser::_parseStatements()
	{
        std::vector<StatementNodePtr> statements;
        auto statement = _parseSingleStatement();
        while (statement)
        {
            //statements.emplace_back(statement);
            statement = _parseSingleStatement();
        }

        return statements;
	}

    NodePtr Parser::_parseSingleStatement()
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

		return nullptr;
	}

    NodePtr Parser::_parseAssignment()
	{
		// TODO use value here.
		if (_parseID())
		{
		    return nullptr;
			/*return
				_match(Token::TokenKind::EQUAL) &&
				_match(Token::TokenKind::NUMBER) &&
				_match(Token::TokenKind::SEMICOLON);*/
		}

		return nullptr;
	}

    std::unique_ptr<BinaryNode> Parser::_parseConditional()
	{
		return nullptr;
	}

    NodePtr Parser::_parseLoop()
	{
		if (_match(Token::TokenKind::KEYWORD, "while") &&
            _match(Token::TokenKind::OPEN_PARENTHESIS))
        {
		    auto condition = _parseConditional();
		    if (_match(Token::TokenKind::CLOSE_PARENTHESIS))
            {
		        auto block = _parseBlock();
                return std::make_unique<ComparisonNode>(std::move(condition), std::move(block));
            }
        }

		return nullptr;
	}

    NodePtr Parser::_parseFunctionCall()
	{
		return nullptr;
	}

    NodePtr Parser::_parseEmptyBlock() const
	{
		return std::make_unique<NullNode>();
	}

	void Parser::_handleError(Token token, Token::TokenKind tokenKind, std::string_view lexeme)
	{
		if (lexeme == "")
		{
            _state.AddError(std::string("Parse error: Expected ") + Token::getNameFromTokenKind(tokenKind));
			return;
		}

        _state.AddError(std::string("Parse error: Got ") + token.getLexeme() + ", expected: " + std::string(lexeme));
	}

	void Parser::_handleEndOfInput(Token::TokenKind tokenKind, std::string_view lexeme)
	{
		if (lexeme == "")
		{
            _state.AddError(std::string("Determined end of input: Expected ") + Token::getNameFromTokenKind(tokenKind));
			return;
		}

        _state.AddError(std::string("Determined end of input, expected ") + std::string(lexeme));
	}
}
