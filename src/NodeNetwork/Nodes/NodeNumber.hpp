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

		void updateValue(const ArduinoJson::JsonObject &nodeData) override;

		void connectOutport(const std::string &portID, Connection<float> &connection) override;

		std::shared_ptr<OutputPort<float>> out;
		float value;
	};
} // namespace Node