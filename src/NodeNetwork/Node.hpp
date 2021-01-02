#pragma once

#include "NodeData.hpp"
#include "Input.hpp"
#include "Output.hpp"

namespace Node
{
	struct Node
	{
		int id;
		NodeData data;
		std::map<std::string, Input> inputs;
		std::map<std::string, Output> outputs;
		std::vector<int> position;
		std::string name;
	};
} // namespace Node
