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
		// int evalNum(const Context &context, const LedContext &ledContext);

		int eval(const Context &context, const LedContext &ledContext);

		// private:
		// OutputPort<int, NodeNumber, &NodeNumer::evalNum> out;
		OutputPort<int> out;
		int value;
	};

	NodeNumber::NodeNumber(/* args */)
	{
	}

	NodeNumber::~NodeNumber()
	{
	}

	int NodeNumber::eval(const Context &context, const LedContext &ledContext)
	{
		return value;
	}

} // namespace Node