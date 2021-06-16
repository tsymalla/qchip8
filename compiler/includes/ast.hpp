#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <string>

namespace compiler
{
    class ExprNode
	{
    public:
        virtual void GenerateCode() = 0;
	};

    using ExprNodePtr = std::unique_ptr<ExprNode>;

    class TopLevelNode: public ExprNode
    {
    public:
        explicit TopLevelNode(std::string name): _name{ std::move(name) }
        {
        }

        void GenerateCode() override
        {
        }

    private:
        std::string _name;
    };

    class VariableNode: public ExprNode
    {
    public:
        explicit VariableNode(std::string name): _name{ std::move(name) }
        {
        }

        void GenerateCode() override
        {
        }

    private:
        std::string _name;
    };

    class UnaryNode: public ExprNode
    {
        explicit UnaryNode(ExprNodePtr node, char op):
            _node{ std::move(node) },
            _operator{ op }
        {
        }

        void GenerateCode() override;

    private:
        ExprNodePtr _node;
        char _operator;
    };

    class BinaryNode: public ExprNode
    {
    public:
        explicit BinaryNode(ExprNodePtr left, ExprNodePtr right, char op):
            _left{ std::move(left)},
            _right{ std::move(right) },
            _operator{ op }
        {
        }

        void GenerateCode() override;

    private:
        ExprNodePtr _left;
        ExprNodePtr _right;
        char _operator;
    };
}

#endif
