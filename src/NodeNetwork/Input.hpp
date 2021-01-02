#pragma once

#include <vector>
#include "Connection.hpp"

namespace Node
{
	struct Input
	{
		std::vector<Connection> connections;
	};
} // namespace Node
