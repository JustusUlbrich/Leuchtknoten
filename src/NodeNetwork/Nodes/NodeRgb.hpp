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

		std::shared_ptr<OutputPort<DataRgb>> out;
		DataRgb value;
	};

	NodeRgb::NodeRgb(/* args */)
	{
		out = std::make_shared<OutputPort<DataRgb>>("num", getptr());
	}

	NodeRgb::~NodeRgb()
	{
	}

	void NodeRgb::eval(const Context &context, const LedContext &ledContext, DataRgb &out)
	{
		out = this->value;
	}

} // namespace Node