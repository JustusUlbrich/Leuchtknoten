#pragma once

#include <string>
#include <vector>
#include <memory>
#include "optional.hpp"

#include "INode.hpp"
#include "Connection.hpp"
#include "../../context.hpp"

// TODO: Think about adding a function as template parameter. And then add eval function for output it

namespace Node
{
	template <typename T>
	class InputPort
	{
	public:
		std::string identifier;
		std::shared_ptr<INode> node;
		nonstd::optional<Connection<T>> connection;
	};

	// template <typename T, typename N, T (N::*func)(const Context&, const LedContext&)>
	template <typename T>
	class OutputPort
	{
	public:
		std::string identifier;
		std::shared_ptr<INode> node;
		std::vector<Connection<T>> connections;

		T eval(const Context &context, const LedContext &ledContext)
		{
			return node->eval<T>(context, ledContext);
			// return (node->*func)(context, ledContext);
		}
	};

} // namespace Node
