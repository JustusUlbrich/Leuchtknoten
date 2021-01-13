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

		InputPort() = delete;

		InputPort(std::string identifier, std::shared_ptr<INode> node)
		: identifier(identifier), node(node)
		{}
	};

	template <typename T>
	class OutputPort
	{
	public:
		std::string identifier;
		std::shared_ptr<INode> node;
		std::vector<Connection<T>> connections;

		OutputPort() = delete;

		OutputPort(std::string identifier, std::shared_ptr<INode> node)
		: identifier(identifier), node(node)
		{}

		T eval(const Context &context, const LedContext &ledContext)
		{
			T out;
			node->eval(context, ledContext, out);
			return out;
		}
	};

} // namespace Node
