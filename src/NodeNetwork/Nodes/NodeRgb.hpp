#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeRgb : public INode
	{

	public:
		explicit NodeRgb(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory);
		~NodeRgb();

		void eval(const Context &context, const LedContext &ledContext, DataRgb &out) override;
		void connectOutport(const std::string &portID, Connection<DataRgb> &connection) override;

		std::shared_ptr<OutputPort<DataRgb>> out;
		DataRgb value;
	};
} // namespace Node