#pragma once

#include "Node.hpp"

namespace Node
{
	struct NodeNetwork
	{
		std::string id;
		std::map<std::string, std::shared_ptr<Node>> nodes;
	};
} // namespace Node
