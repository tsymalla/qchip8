#ifndef PARSER_HPP
#define PARSER_HPP

#include <lexer.hpp>
#include <ast.hpp>
#include <astvisitor.hpp>
#include <vector>
#include <utility>

namespace compiler
{
    class ParserState final
    {
    public:
        void AddError(std::string message)
        {
            _hasError = true;
            _errors.push_back(std::move(message));
        }

        bool HasError() const
        {
            return _hasError;
        }

        std::vector<std::string> GetErrorMessages() const
        {
            return _errors;
        }

        Node* GetAstRoot()
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
        bool _hasError = false;
        std::vector<std::string> _errors;
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
        std::unique_ptr<BlockNode> _parseBlock();
        std::vector<NodePtr> _parseStatements();
        NodePtr _parseSingleStatement();
        NodePtr _parseAssignment();
        std::unique_ptr<BinaryNode> _parseIf();
        std::unique_ptr<BinaryNode> _parseCondition();
        NodePtr _parseLoop();
        NodePtr _parseFunctionCall();
        NodePtr _parseEmptyBlock() const;

        void _handleError(const Token& token, Token::TokenKind tokenKind, std::string_view lexeme = "");
        void _handleEndOfInput(Token::TokenKind tokenKind, std::string_view lexeme = "");

    public:
		Parser(const std::vector<Token>& tokens);
		
		Parser(const Parser&) = delete;
		Parser& operator=(const Parser&) = delete;
		Parser(Parser&&) = delete;
		Parser& operator=(Parser&&) = delete;

        ParserState const* Parse();
	};
}

#endif
