#pragma once

#include <map>

#include "../NodeNetwork.hpp"
#include "../../context.hpp"

#include "../NodeData.hpp"
#include "../Input.hpp"
#include "../Output.hpp"

namespace Node
{
	class INode
	{
	public:
		virtual NodeData eval(
			const Context &context,
			const LedContext &ledContext,
			const Output &target,
			const NodeNetwork &network) = 0;

		// TODO: private?
		int id;
		NodeData data;
		std::map<std::string, Input> inputs;
		std::map<std::string, Output> outputs;
		std::vector<int> position;
		std::string name;
	};

} // namespace Node