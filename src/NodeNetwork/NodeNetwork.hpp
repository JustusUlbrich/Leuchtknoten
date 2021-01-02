#pragma once

#include "Node.hpp"

namespace Node
{
	struct NodeNetwork
	{
		std::string id;
		std::map<std::string, Node> nodes;
	};
} // namespace Node
