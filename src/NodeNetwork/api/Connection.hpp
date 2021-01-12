#pragma once

#include <memory>

// #include "Port.hpp"
// #include "ConnectionData.hpp"

namespace Node
{
	template <typename X>
	class OutputPort;

	template <typename X>
	class InputPort;

	template <typename T>
	class Connection
	{
	private:
	public:
		std::shared_ptr<OutputPort<T>> fromPort;
		std::shared_ptr<InputPort<T>> toPort;

		// std::string port;
		// ConnectionData data;
		// T data;
	};
} // namespace Node
