#ifndef ASTVISITOR_HPP
#define ASTVISITOR_HPP

#include <iostream>

namespace compiler
{
    class ProgramNode;
    class BlockNode;
    class StringLiteralNode;
    class NumericLiteralNode;
    class UnaryNode;
    class BinaryNode;
    class ComparisonNode;

    class NodeVisitor
    {
    public:
        virtual void VisitProgram(ProgramNode* node) = 0;
        virtual void VisitBlock(BlockNode* node) = 0;
        virtual void VisitString(StringLiteralNode* node) = 0;
        virtual void VisitNumeric(NumericLiteralNode* node) = 0;
        virtual void VisitUnary(UnaryNode* node) = 0;
        virtual void VisitBinary(BinaryNode* node) = 0;
        virtual void VisitComparison(ComparisonNode* node) = 0;
    };

    class PrintNodeVisitor final : public NodeVisitor
    {
    public:
        void VisitProgram(ProgramNode* node) override;
        void VisitBlock(BlockNode* node) override;
        void VisitString(StringLiteralNode* node) override;
        void VisitNumeric(NumericLiteralNode* node) override;
        void VisitUnary(UnaryNode* node) override;
        void VisitBinary(BinaryNode* node) override;
        void VisitComparison(ComparisonNode* node) override;
    };
}
#endif