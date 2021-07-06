#ifndef PARSER_HPP
#define PARSER_HPP

#include <ast.hpp>
#include <astvisitor.hpp>
#include <lexer.hpp>
#include <parser_error_state.hpp>
#include <utility>
#include <vector>

namespace compiler
{
	class ParserState final
	{
	  public:
		ParserErrorState* GetErrorState()
		{
			return &_errorState;
		}

		Node* GetAstRoot() const
		{
			return _astRoot.get();
		}

		void SetAstRoot(NodePtr root)
		{
			_astRoot = std::move(root);
		}

		void Dump() const
		{
			PrintNodeVisitor visitor;
			_astRoot->Accept(&visitor);
		}

	  private:
		ParserErrorState _errorState;
		NodePtr _astRoot;
	};

	class Parser final
	{
		int _tokenIndex;
		std::vector<Token> _tokens;
		const Token NullToken{Token::TokenKind::UNDEFINED, "", 0, 0};
		ParserState _state;

		bool _hasToken() const;

		Token _getCurrentToken() const;

		bool _match(Token::TokenKind tokenKind);

		bool _match(Token::TokenKind tokenKind, std::string_view lexeme);

		Token _peek() const;

		Token _expect(Token::TokenKind tokenKind);

		void _advance();

		NodePtr _getID();

		NodePtr _getNumericLiteral();

		NodePtr _parseProgram();

		NodePtr _parseExpression();

		std::unique_ptr<BlockNode> _parseBlock();

		std::vector<NodePtr> _parseStatements();

		NodePtr _parseSingleStatement();

		NodePtr _parseAssignment();

		std::unique_ptr<BinaryNode> _parseIf();

		std::unique_ptr<BinaryNode> _parseCondition();

		NodePtr _parseLoop();

		NodePtr _parseFunctionCall();

	  public:
		Parser(const std::vector<Token>& tokens);

		Parser(const Parser&) = delete;

		Parser& operator=(const Parser&) = delete;

		Parser(Parser&&) = delete;

		Parser& operator=(Parser&&) = delete;

		ParserState* Parse();
	};
} // namespace compiler

#endif
