#pragma once

#include <vector>

#include "../../context.hpp"

// #include "NodeData.hpp"

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

		template <typename T>
		T eval(const Context &context, const LedContext &ledContext);
	};

} // namespace Node