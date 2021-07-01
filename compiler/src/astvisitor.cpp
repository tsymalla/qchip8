#include <astvisitor.hpp>
#include <ast.hpp>

namespace compiler
{
    void PrintNodeVisitor::VisitProgram(ProgramNode* node)
    {
        VisitString(dynamic_cast<StringLiteralNode*>(node->GetName()));

        for (const auto& block : node->GetBlocks())
        {
            VisitBlock(block.get());
        }
    }

    void PrintNodeVisitor::VisitBlock(BlockNode* node)
    {
    }

    void PrintNodeVisitor::VisitString(StringLiteralNode* node)
    {
        std::cout << "String: " << node->GetValue() << std::endl;
    }

    void PrintNodeVisitor::VisitNumeric(NumericLiteralNode* node)
    {
        std::cout << "Numeric: " << node->GetValue() << std::endl;
    }

    void PrintNodeVisitor::VisitUnary(UnaryNode* node)
    {
    	
    }
	
    void PrintNodeVisitor::VisitBinary(BinaryNode* node)
    {
	    
    }
	
    void PrintNodeVisitor::VisitComparison(ComparisonNode* node)
    {
	    
    }
}