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
		INode *node;
		nonstd::optional<Connection<T>> connection;

		InputPort() = delete;

		InputPort(const std::string &identifier, INode *node)
			: identifier(identifier), node(node)
		{
		}
	};

	template <typename T>
	class OutputPort
	{
	public:
		std::string identifier = "";
		INode *node = nullptr;
		std::vector<Connection<T>> connections;

		OutputPort() = delete;

		OutputPort(const std::string &identifier, INode *_node)
			: identifier(identifier), node(_node)
		{
			debugOut("\t\t\t Create Port ");
			debugOut(identifier.c_str());
			debugOut(" with Node ");
			debugOutln(node->name.c_str());
		}

		T eval(const Context &context, const LedContext &ledContext)
		{
			debugOut("\t\t\t Eval Port: ");
			debugOutln(identifier.c_str());

			// debugOutln("\t\t\t at Node: ");
			// debugOutln(node->id);

			T out = T();
			if (node != nullptr)
				node->eval(context, ledContext, identifier, out);
			else
				debugOutln("\t\t\t Port node empty :(");
			return out;
		}
	};

} // namespace Node
