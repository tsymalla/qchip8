#ifndef AST_HPP
#define AST_HPP

#include <astvisitor.hpp>
#include <lexer.hpp>
#include <memory>
#include <string>
#include <vector>

namespace compiler
{
	class Node
	{
	  public:
		virtual void GenerateCode() = 0;
		virtual void Accept(NodeVisitor* visitor) = 0;
	};

	using NodePtr = std::unique_ptr<Node>;

	class NullNode final : public Node
	{
	  public:
		void GenerateCode() override
		{
		}
		void Accept(NodeVisitor* visitor) override
		{
		}
	};

	class StatementNode : public Node
	{
	  public:
		void GenerateCode() override
		{
		}
		void Accept(NodeVisitor* visitor) override
		{
		}
	};

	class ExpressionNode : public Node
	{
	  public:
		ExpressionNode() = default;
		void GenerateCode() override
		{
		}
		void Accept(NodeVisitor* visitor) override
		{
		}
	};

	using StatementNodePtr = std::unique_ptr<StatementNode>;
	using ExpressionNodePtr = std::unique_ptr<ExpressionNode>;

	class LiteralNode : public ExpressionNode
	{
	  public:
		LiteralNode() = default;
		void GenerateCode() override
		{
		}
		void Accept(NodeVisitor* visitor) override
		{
		}
	};

	class NumericLiteralNode : public LiteralNode
	{
	  public:
		explicit NumericLiteralNode(int value) : _value{value}
		{
		}

		int GetValue() const
		{
			return _value;
		}

		void GenerateCode() override
		{
		}
		void Accept(NodeVisitor* visitor) override;

	  private:
		int _value;
	};

	class StringLiteralNode : public LiteralNode
	{
	  public:
		explicit StringLiteralNode(std::string value) : _value{std::move(value)}
		{
		}

		std::string GetValue() const
		{
			return _value;
		}

		void GenerateCode() override
		{
		}
		void Accept(NodeVisitor* visitor) override;

	  private:
		std::string _value;
	};

	class BlockNode : public Node
	{
	  public:
		explicit BlockNode(std::vector<NodePtr>&& statements) : _statements{std::move(statements)}
		{
		}

		const std::vector<NodePtr>& GetStatements() const
		{
			return _statements;
		}

		void GenerateCode() override
		{
		}
		void Accept(NodeVisitor* visitor) override;

	  private:
		std::vector<NodePtr> _statements;
	};

	class ProgramNode : public Node
	{
	  public:
		explicit ProgramNode(NodePtr name) : _name{std::move(name)}
		{
		}

		void AddBlock(std::unique_ptr<BlockNode> block)
		{
			_blocks.push_back(std::move(block));
		}

		Node* GetName() const
		{
			return _name.get();
		}

		const std::vector<std::unique_ptr<BlockNode>>& GetBlocks() const
		{
			return _blocks;
		}

		void GenerateCode() override
		{
		}

		void Accept(NodeVisitor* visitor) override;

	  private:
		NodePtr _name;
		std::vector<std::unique_ptr<BlockNode>> _blocks;
	};

	enum class UnaryOperator : uint8_t
	{
		PLUS,
		MINUS,
		NOT,
		COMPLEMENT
	};

	class UnaryNode : public ExpressionNode
	{
		explicit UnaryNode(ExpressionNodePtr node, UnaryOperator op) : _node{std::move(node)}, _operator{op}
		{
		}

		Node* GetNode() const
		{
			return _node.get();
		}

		UnaryOperator GetOperator() const
		{
			return _operator;
		}

		void GenerateCode() override;
		void Accept(NodeVisitor* visitor) override;

	  private:
		ExpressionNodePtr _node;
		UnaryOperator _operator;
	};

	enum class BinaryOperator : uint8_t
	{
		PLUS,
		MINUS,
		TIMES,
		DIVIDE,
		MODULO,
		AND,
		OR,
		XOR,
		ASSIGN,
		LESS,
		LESS_OR_EQUAL,
		GREATER,
		GREATER_OR_EQUAL,
		EQUAL
	};

	class BinaryNode : public ExpressionNode
	{
	  public:
		explicit BinaryNode(NodePtr left, NodePtr right, BinaryOperator op)
			: _left{std::move(left)}, _right{std::move(right)}, _operator{op}
		{
		}

		~BinaryNode() = default;

		static BinaryOperator GetBinaryOperator(Token token)
		{
			switch (token.getKind())
			{
			case Token::TokenKind::GREATER:
				return BinaryOperator::GREATER;
			case Token::TokenKind::GREATER_OR_EQUAL:
				return BinaryOperator::GREATER_OR_EQUAL;
			case Token::TokenKind::LESS:
				return BinaryOperator::LESS;
			case Token::TokenKind::LESS_OR_EQUAL:
				return BinaryOperator::LESS_OR_EQUAL;
			case Token::TokenKind::EQUAL:
				return BinaryOperator::EQUAL;
			}
		}

		static const char* BinaryOperatorToString(BinaryOperator op)
		{
			switch (op)
			{
			case BinaryOperator::AND:
				return "&";
			case BinaryOperator::ASSIGN:
				return "=";
			case BinaryOperator::DIVIDE:
				return "/";
			case BinaryOperator::EQUAL:
				return "==";
			case BinaryOperator::GREATER:
				return ">";
			case BinaryOperator::GREATER_OR_EQUAL:
				return ">=";
			case BinaryOperator::LESS:
				return "<";
			case BinaryOperator::LESS_OR_EQUAL:
				return "<=";
			case BinaryOperator::MINUS:
				return "-";
			case BinaryOperator::MODULO:
				return "%";
			case BinaryOperator::OR:
				return "|";
			case BinaryOperator::PLUS:
				return "+";
			case BinaryOperator::TIMES:
				return "*";
			case BinaryOperator::XOR:
				return "^";
			}
		}

		Node* GetLHS() const
		{
			return _left.get();
		}

		Node* GetRHS() const
		{
			return _right.get();
		}

		BinaryOperator GetOperator() const
		{
			return _operator;
		}

		void GenerateCode() override
		{
		}
		
		void Accept(NodeVisitor* visitor) override;

	  private:
		NodePtr _left;
		NodePtr _right;
		BinaryOperator _operator;
	};

	class ComparisonNode : public Node
	{
	  public:
		explicit ComparisonNode(std::unique_ptr<BinaryNode> condition, std::unique_ptr<BlockNode> block)
			: _condition{std::move(condition)}, _block{std::move(block)}
		{
		}

		BinaryNode* GetCondition() const
		{
			return _condition.get();
		}

		BlockNode* GetBlock() const
		{
			return _block.get();
		}

		void GenerateCode() override
		{
		}

		void Accept(NodeVisitor* visitor) override;

	  private:
		std::unique_ptr<BinaryNode> _condition;
		std::unique_ptr<BlockNode> _block;
	};
} // namespace compiler

#endif