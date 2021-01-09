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

		int eval(const OutputPort<int, NodeNumber> &out, const Context &context, const LedContext &ledContext);

	private:
		// OutputPort<int, NodeNumber, &NodeNumer::evalNum> out;
		OutputPort<int, NodeNumber> out;
	};

	NodeNumber::NodeNumber(/* args */)
	{
	}

	NodeNumber::~NodeNumber()
	{
	}

	int NodeNumber::eval(const OutputPort<int, NodeNumber> &out, const Context &context, const LedContext &ledContext)
	{
		auto data = nonstd::get<int>(this->data.value);
		return data;
	}

} // namespace Node