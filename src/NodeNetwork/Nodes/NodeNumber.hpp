#pragma once

#include "INode.hpp"

namespace Node
{
	class NodeNumber : public INode
	{
	public:
		NodeData eval(
			const Context &context,
			const LedContext &ledContext,
			const Output &target,
			const NodeNetwork &network) override
		{
			auto data = this->data;
			auto num = data.num;
			return data;
		}
	};
} // namespace Node