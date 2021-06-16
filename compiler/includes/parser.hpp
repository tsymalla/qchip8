#ifndef PARSER_HPP
#define PARSER_HPP

#include <lexer.hpp>
#include <ast.hpp>
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

        ExprNode* GetAstRoot()
        {
            return _astRoot.get();
        }

        void SetAstRoot(ExprNodePtr root)
        {
            _astRoot = std::move(root);
        }

    private:
        bool _hasError = false;
        std::vector<std::string> _errors;
        ExprNodePtr _astRoot;
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
        void _advance();
        bool _parseID();
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

        ParserState const* Parse();
	};
}

#endif
