#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeNumber : public INode
	{

	public:
		explicit NodeNumber(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory);
		~NodeNumber();

		void eval(const Context &context, const LedContext &ledContext, int &out) override;
		void connectOutport(const std::string &portID, Connection<int> &connection) override;

		std::shared_ptr<OutputPort<int>> out;
		int value;
	};
} // namespace Node