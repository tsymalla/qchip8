#ifndef PARSER_HPP
#define PARSER_HPP

#include <lexer.hpp>
#include <vector>

namespace compiler
{
/*
    class AstNode
	{
    public:
        virtual void GenerateCode() = 0;
	};

    class AstRootNode: public AstNode
    {
        std::vector<AstRootNode*> children;

        // AstNode interface
    public:
        void GenerateCode() override;
    };

    class AstBinaryNode: public AstNode
    {
        Variable a;
        Variable b;
        Operator operator;

        // AstNode interface
        public:
            void GenerateCode() override;
    };

    class AstComparatorNode: public AstNode
    {
        AstBinaryNode* Comparator = nullptr;
        AstNode* If = nullptr;
        AstNode* Else = nullptr;

        // AstNode interface
    public:
        void GenerateCode() override;
    };

    class AstWhileNode: public AstNode
    {
        AstBinaryNode* Comparator = nullptr;
        AstRootNode* Body = nullptr;

        // AstNode interface
    public:
        void GenerateCode() override;
    };
*/

	class Parser final
	{
		int _tokenIndex;
		std::vector<Token> _tokens;
		std::vector<std::string> _errors;
		const Token NullToken{Token::TokenKind::UNDEFINED, "", 0, 0};

		bool _hasToken() const;
		Token _getCurrentToken() const;

        bool _match(Token::TokenKind tokenKind);
		bool _match(Token::TokenKind tokenKind, std::string_view lexeme);
        bool _parseProgram();
        bool _parseBlock();
        bool _parseStatements();
        bool _parseSingleStatement();
        bool _parseAssignment();
        bool _parseConditional();
        bool _parseLoop();
        bool _parseFunctionCall();
        bool _parseEmptyBlock() const;

        void _handleError(Token token, Token::TokenKind tokenKind, std::string_view lexeme = "");
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
