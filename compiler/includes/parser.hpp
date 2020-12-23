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
		std::vector<Token> _tokens;
	public:
		Parser(const std::vector<Token> tokens);
	};
}

#endif