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

		Token _getCurrentToken() const;

		void _match(Token::TokenKind tokenKind);
		void _parseExpression();
		void _parseConstant();
		void _parseStringLiteral();

		void _handleError(Token token);

	public:
		Parser(const std::vector<Token>& tokens);
		
		Parser(const Parser&) = delete;
		Parser& operator=(const Parser&) = delete;
		Parser(Parser&&) = delete;
		Parser& operator=(Parser&&) = delete;

		void Parse();
	};
}

#endif