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

		void eval(const Context &context, const LedContext &ledContext, DataRgb &out) override;

		OutputPort<int> out;
		DataRgb value;
	};

	NodeRgb::NodeRgb(/* args */)
	{
	}

	NodeRgb::~NodeRgb()
	{
	}

	void NodeRgb::eval(const Context &context, const LedContext &ledContext, DataRgb &out)
	{
		out = this->value;
	}

} // namespace Node