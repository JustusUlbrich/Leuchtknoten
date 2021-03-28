#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeBool : public INode
	{

	public:
		explicit NodeBool(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory);
		~NodeBool();

		void updateValue(const ArduinoJson::JsonObject &nodeData) override;

		void connectOutport(const std::string &portID, Connection<bool> &connection) override;

		std::shared_ptr<OutputPort<bool>> out;
		bool value;
	};
} // namespace Node