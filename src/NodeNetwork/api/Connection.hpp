#pragma once

#include <memory>

#include "INode.hpp"
#include "ConnectionData.hpp"

namespace Node
{
	template <typename T>
	class Connection
	{
		std::shared_ptr<INode> fromPort;
		std::shared_ptr<INode> toPort;

		// std::string port;
		// ConnectionData data;
		// T data;
	};
} // namespace Node
