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

		void updateValue(const ArduinoJson::JsonObject &nodeData) override;
		void connectOutport(const std::string &portID, Connection<CRGB> &connection) override;

		std::shared_ptr<OutputPort<CRGB>> out;
		CRGB value;
	};
} // namespace Node