#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeRgb : public INode
	{

	public:
		NodeRgb(/* args */);
		~NodeRgb();

		DataRgb eval(const OutputPort<int, NodeRgb> &out, const Context &context, const LedContext &ledContext);

		OutputPort<int, NodeRgb> out;
		DataRgb value;
	};

	NodeRgb::NodeRgb(/* args */)
	{
	}

	NodeRgb::~NodeRgb()
	{
	}

	DataRgb NodeRgb::eval(const OutputPort<int, NodeRgb> &out, const Context &context, const LedContext &ledContext)
	{
		return this->value;
	}

} // namespace Node