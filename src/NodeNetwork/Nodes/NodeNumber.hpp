#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeNumber : public INode
	{

	public:
		NodeNumber(/* args */);
		~NodeNumber();

		void eval(const Context &context, const LedContext &ledContext, int &out) override;

		OutputPort<int> out;
		int value;
	};

	NodeNumber::NodeNumber(/* args */)
	{
	}

	NodeNumber::~NodeNumber()
	{
	}

	void NodeNumber::eval(const Context &context, const LedContext &ledContext, int &out)
	{
		out = value;
	}

} // namespace Node