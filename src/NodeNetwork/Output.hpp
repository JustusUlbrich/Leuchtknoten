#pragma once

#include <vector>
#include "Connection.hpp"

namespace Node
{
	struct Output
	{
		std::shared_ptr<Connection> connection;
	};
} // namespace Node
