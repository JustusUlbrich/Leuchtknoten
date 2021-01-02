#pragma once

#include <string>
#include "ConnectionData.hpp"

namespace Node
{
	struct Connection
	{
		int node;
		std::string output;
		ConnectionData data;
	};
} // namespace Node
