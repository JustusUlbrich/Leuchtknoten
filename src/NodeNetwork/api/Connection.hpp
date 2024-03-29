#pragma once

#include <memory>

// #include "Port.hpp"
// #include "ConnectionData.hpp"

namespace Node
{
	template <typename T>
	class OutputPort;

	template <typename X>
	class InputPort;

	template <typename T>
	class Connection
	{
	private:
	public:
		std::shared_ptr<OutputPort<T>> fromPort = nullptr;
		std::shared_ptr<InputPort<T>> toPort = nullptr;

		// std::string port;
		// ConnectionData data;
		// T data;
	};
} // namespace Node
