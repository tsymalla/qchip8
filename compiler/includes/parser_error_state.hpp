#ifndef PARSER_ERROR_STATE_HPP
#define PARSER_ERROR_STATE_HPP

#include <lexer.hpp>
#include <string_view>
#include <vector>
#include <ostream>

namespace compiler
{
	enum class PARSE_ERROR_TYPE
	{
		UNEXPECTED_TOKEN,
		END_OF_INPUT
	};
	
	class ParseError
	{
	public:
		explicit ParseError(PARSE_ERROR_TYPE type);

		void SetToken(const Token& token);
		void SetExpectedTokenKind(Token::TokenKind tokenKind);
		void SetLexeme(std::string_view lexeme);
		
		void DumpError(std::ostream& out) const;

	private:
		PARSE_ERROR_TYPE _type;
		Token _token;
		Token::TokenKind _expectedTokenKind{};
		std::string_view _lexeme{};
	};
	
	class ParserErrorState final
	{
	public:
		void HandleUnexpectedToken(const Token& token, Token::TokenKind tokenKind, std::string_view lexeme = "");
		void HandleEndOfInput(Token::TokenKind tokenKind, std::string_view lexeme = "");

		bool HasErrors() const
		{
			return !_errorData.empty();
		}
		
		void DumpErrors(std::ostream& out) const;

	private:
		std::vector<ParseError> _errorData;
	};
}

#endif