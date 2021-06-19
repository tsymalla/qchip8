#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <string>

namespace compiler
{
    class Node
	{
    public:
        virtual void GenerateCode() = 0;
	};

    using NodePtr = std::unique_ptr<Node>;

    class NullNode final: public Node
    {
    public:
        void GenerateCode() override {}
    };

    class StatementNode: public Node
    {
    public:
        void GenerateCode() override {}
    };

    class ExpressionNode: public Node
    {
    public:
        ExpressionNode() = default;
        void GenerateCode() override {}
    };

    using StatementNodePtr = std::unique_ptr<StatementNode>;
    using ExpressionNodePtr = std::unique_ptr<ExpressionNode>;

    class LiteralNode: public ExpressionNode
    {
    public:
        LiteralNode() = default;
        void GenerateCode() override {}
    };

    class NumericLiteralNode: public LiteralNode
    {
    public:
        explicit NumericLiteralNode(int value): _value{ value }
        {
        }

        void GenerateCode() override {}

    private:
        int _value;
    };

    class StringLiteralNode: public LiteralNode
    {
    public:
        explicit StringLiteralNode(std::string value): _value{ std::move(value) }
        {
        }

        void GenerateCode() override {}

    private:
        std::string _value;
    };

    class BlockNode: public Node
    {
    public:
        explicit BlockNode(const std::vector<StatementNodePtr>& statements)
        {
        }

        void GenerateCode() override {}
    private:
        std::vector<StatementNodePtr> _statements;
    };

    class ProgramNode: public Node
    {
    public:
        explicit ProgramNode(NodePtr name): _name{ std::move(name) }
        {
        }

        void AddBlock(std::unique_ptr<BlockNode> block)
        {
            _blocks.push_back(std::move(block));
        }

        void GenerateCode() override
        {
        }

    private:
        NodePtr _name;
        std::vector<std::unique_ptr<BlockNode>> _blocks;
    };

    enum class UnaryOperator: uint8_t
    {
        PLUS,
        MINUS,
        NOT,
        COMPLEMENT
    };

    class UnaryNode: public ExpressionNode
    {
        explicit UnaryNode(ExpressionNodePtr node, UnaryOperator op):
            _node{ std::move(node) },
            _operator{ op }
        {
        }

        void GenerateCode() override;

    private:
        ExpressionNodePtr _node;
        UnaryOperator _operator;
    };

    enum class BinaryOperator: uint8_t
    {
        PLUS,
        MINUS,
        TIMES,
        DIVIDE,
        MODULO,
        AND,
        OR,
        XOR,
        ASSIGN
    };

    class BinaryNode: public ExpressionNode
    {
    public:
        explicit BinaryNode(ExpressionNodePtr left, ExpressionNodePtr right, BinaryOperator op):
            _left{ std::move(left)},
            _right{ std::move(right) },
            _operator{ op }
        {
        }

        ~BinaryNode() = default;
        void GenerateCode() override {}

    private:
        ExpressionNodePtr _left;
        ExpressionNodePtr _right;
        BinaryOperator _operator;
    };

    class ComparisonNode: public Node
    {
    public:
        explicit ComparisonNode(std::unique_ptr<BinaryNode> condition, std::unique_ptr<BlockNode> block): _condition{ std::move(condition) }, _block{ std::move(block) }
        {
        }

        void GenerateCode() override
        {
        }
    private:
        std::unique_ptr<BinaryNode> _condition;
        std::unique_ptr<BlockNode> _block;
    };
}

#endif
