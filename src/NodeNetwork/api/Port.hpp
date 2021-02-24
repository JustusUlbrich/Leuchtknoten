#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

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

		typedef std::function<T(const Context&, const LedContext &ledContext)> EvalFunc;
		const EvalFunc evalNodeFunc;

		OutputPort() = delete;

		OutputPort(const std::string &identifier, INode *_node, EvalFunc _evalNodeFunc)
			: identifier(identifier), node(_node), evalNodeFunc(_evalNodeFunc)
		{
			debugOut("\t\t\t Create Port ");
			debugOut(identifier.c_str());
			debugOut(" with Node ");
			debugOutln(node->name.c_str());
		}

		inline T eval(const Context &context, const LedContext &ledContext)
		{
			debugOut("\t\t\t Eval Port: ");
			debugOutln(identifier.c_str());

			// debugOutln("\t\t\t at Node: ");
			// debugOutln(node->id);

			T out = T();
			if (node != nullptr && evalNodeFunc != nullptr)
				out = evalNodeFunc(context, ledContext);
			else
				debugOutln("\t\t\t Port node empty :(");
			return out;
		}
	};

} // namespace Node
