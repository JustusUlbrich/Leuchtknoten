#pragma once

#include <string>
#include <vector>
#include <memory>
#include "optional.hpp"

#include "Connection.hpp"
#include "../../context.hpp"

// TODO: Think about adding a function as template parameter. And then add eval function for output it

namespace Node
{
	template <typename T, typename N>
	class InputPort
	{
		std::shared_ptr<N> node;
		nonstd::optional<Connection<T>> connection;
	};

	// template <typename T, typename N, T (N::*func)(const Context&, const LedContext&)>
	template <typename T, typename N>
	class OutputPort
	{
		std::string identifier;
		std::shared_ptr<N> node;
		std::vector<Connection<T>> connections;

		T eval(const Context &context, const LedContext &ledContext)
		{
			return node->eval(this, context, ledContext);
			// return (node->*func)(context, ledContext);
		}
	};
} // namespace Node
