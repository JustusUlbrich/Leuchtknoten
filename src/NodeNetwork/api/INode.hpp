#pragma once

#include <vector>

#include "NodeData.hpp"

namespace Node
{
	class INode
	{
	public:
		// TODO: private?
		int id;
		// NodeData data;
		std::vector<int> position;
		std::string name;
	};

} // namespace Node