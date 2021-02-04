#ifndef PARSER_HPP
#define PARSER_HPP

#include <lexer.hpp>

namespace compiler
{
	struct AstNode
	{
	};

	struct AstBinaryNode
	{

	};

	struct AstUnaryNode
	{

	};

	class Parser final
	{
		int _tokenIndex;
		std::vector<Token> _tokens;
		std::vector<std::string> _errors;

		bool _hasToken() const;
		Token _getCurrentToken() const;

        void _match(Token::TokenKind tokenKind);
		void _match(Token::TokenKind tokenKind, std::string_view lexeme);
		void _parseProgram();
		void _parseStatements();
		void _parseExpression();
		void _parseConstant();
		void _parseStringLiteral();

		void _handleError(Token token, std::string_view lexeme = "");
		void _handleEndOfInput(Token::TokenKind tokenKind, std::string_view lexeme = "");

	public:
		Parser(const std::vector<Token>& tokens);
		
		Parser(const Parser&) = delete;
		Parser& operator=(const Parser&) = delete;
		Parser(Parser&&) = delete;
		Parser& operator=(Parser&&) = delete;

		bool Parse();
	};
}

#endif