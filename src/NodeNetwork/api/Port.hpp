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

		InputPort(std::string identifier, INode *node)
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

		OutputPort(std::string _identifier, INode *_node)
			: identifier(_identifier), node(_node)
		{
			Serial.printf("\t\t\t Create Port %s with Node %s \n", identifier.c_str(), node->name.c_str());
		}

		T eval(const Context &context, const LedContext &ledContext)
		{
			Serial.print("\t\t\t Eval Port: ");
			Serial.println(identifier.c_str());

			Serial.println("\t\t\t at Node: ");
			Serial.println(node->id);

			T out = T();
			// if (node != nullptr)
			// 	node->eval(context, ledContext, out);
			// else
			// 	Serial.println("\t\t\t Port node empty :(");
			return out;
		}
	};

} // namespace Node
