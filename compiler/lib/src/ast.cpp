#include <ast.hpp>

namespace compiler
{
	void NumericLiteralNode::Accept(NodeVisitor* visitor)
	{
		visitor->VisitNumeric(this);
	}

	void StringLiteralNode::Accept(NodeVisitor* visitor)
	{
		visitor->VisitString(this);
	}

	void BlockNode::Accept(NodeVisitor* visitor)
	{
		visitor->VisitBlock(this);
	}

	void ProgramNode::Accept(NodeVisitor* visitor)
	{
		visitor->VisitProgram(this);
	}

	void UnaryNode::Accept(NodeVisitor* visitor)
	{
		visitor->VisitUnary(this);
	}

	void BinaryNode::Accept(NodeVisitor* visitor)
	{
		visitor->VisitBinary(this);
	}

	void ComparisonNode::Accept(NodeVisitor* visitor)
	{
		visitor->VisitComparison(this);
	}
}