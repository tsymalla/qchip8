#include <ast.hpp>
#include <astvisitor.hpp>

namespace compiler
{
	void PrintNodeVisitor::VisitProgram(ProgramNode* node)
	{
		VisitString(dynamic_cast<StringLiteralNode*>(node->GetName()));

		for (const auto& block : node->GetBlocks())
		{
			if (block)
			{
				VisitBlock(block.get());
			}
		}
	}

	void PrintNodeVisitor::VisitBlock(BlockNode* node)
	{
		for (const auto& statement : node->GetStatements())
		{
			statement->Accept(this);
		}
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
		std::cout << "Binary node\n";
		node->GetLHS()->Accept(this);
		std::cout << BinaryNode::BinaryOperatorToString(node->GetOperator()) << std::endl;
		node->GetRHS()->Accept(this);
	}

	void PrintNodeVisitor::VisitComparison(ComparisonNode* node)
	{
	}
} // namespace compiler