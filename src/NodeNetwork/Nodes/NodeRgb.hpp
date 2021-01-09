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

		int eval(const OutputPort<int, NodeRgb> &out, const Context &context, const LedContext &ledContext);

	private:
		OutputPort<int, NodeRgb> out;
	};

	NodeRgb::NodeRgb(/* args */)
	{
	}

	NodeRgb::~NodeRgb()
	{
	}

	int NodeRgb::eval(const OutputPort<int, NodeRgb> &out, const Context &context, const LedContext &ledContext)
	{
		auto data = nonstd::get<DataRgb>(this->data.value);
		return data;
	}

} // namespace Node