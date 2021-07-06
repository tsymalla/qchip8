#include <parser_error_state.hpp>

namespace compiler
{
	ParseError::ParseError(PARSE_ERROR_TYPE type) : _type{type}
	{
	}

	void ParseError::SetToken(const Token& token)
	{
		_token = token;
	}

	void ParseError::SetExpectedTokenKind(Token::TokenKind tokenKind)
	{
		_expectedTokenKind = tokenKind;
	}

	void ParseError::SetLexeme(std::string_view lexeme)
	{
		_lexeme = lexeme;
	}

	void ParseError::DumpError(std::ostream& out) const
	{
		if (_type == PARSE_ERROR_TYPE::UNEXPECTED_TOKEN)
		{
			out << "Parse error: ";

			if (_lexeme.empty())
			{
				out << "Expected " << Token::getNameFromTokenKind(_expectedTokenKind);
			}
			else
			{
				out << "Got " << _token.getLexeme() << ", expected: " << _lexeme;
			}
		}
		else if (_type == PARSE_ERROR_TYPE::END_OF_INPUT)
		{
			out << "End of input: ";
			if (_lexeme.empty())
			{
				out << "Expected " << Token::getNameFromTokenKind(_expectedTokenKind);
			}
			else
			{
				out << "Expected " << _lexeme;
			}
		}
	}

	void ParserErrorState::HandleUnexpectedToken(const Token& token, Token::TokenKind tokenKind,
	                                             std::string_view lexeme)
	{
		ParseError err{PARSE_ERROR_TYPE::UNEXPECTED_TOKEN};
		err.SetToken(token);
		err.SetExpectedTokenKind(tokenKind);
		err.SetLexeme(lexeme);

		_errorData.emplace_back(std::move(err));
	}

	void ParserErrorState::HandleEndOfInput(Token::TokenKind tokenKind, std::string_view lexeme)
	{
		ParseError err{PARSE_ERROR_TYPE::UNEXPECTED_TOKEN};
		err.SetExpectedTokenKind(tokenKind);
		err.SetLexeme(lexeme);

		_errorData.emplace_back(std::move(err));
	}

	void ParserErrorState::DumpErrors(std::ostream& out) const
	{
		if (!HasErrors())
		{
			return;
		}

		out << "Compilation failed. Found " << _errorData.size() << " errors.\n";

		for (const auto& error : _errorData)
		{
			error.DumpError(out);

			out << "\n";
		}

		out.flush();
	}
} // namespace compiler
