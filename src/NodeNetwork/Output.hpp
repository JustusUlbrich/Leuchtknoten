#pragma once

#include <vector>
#include "Connection.hpp"

namespace Node
{
	struct Output
	{
		std::vector<Connection> connections;
	};
} // namespace Node
