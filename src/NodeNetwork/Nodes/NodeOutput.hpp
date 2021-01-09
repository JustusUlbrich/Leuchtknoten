#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeOutput : public INode
	{

	public:
		int eval(const OutputPort<int, NodeOutput> &out, const Context &context, const LedContext &ledContext);

	private:
		InputPort<DataRgb, NodeOutput> out;
	};

} // namespace Node