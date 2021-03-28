#include "NodeBool.hpp"

namespace Node
{
	NodeBool::NodeBool(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		out = std::make_shared<OutputPort<bool>>(
			"bool",
			this,
			[this](const Context &c, const LedContext &lc) { return value; });

		value = nodeJson["data"]["value"] | false;
	}

	NodeBool::~NodeBool()
	{
	}

	void NodeBool::updateValue(const ArduinoJson::JsonObject &nodeData)
	{
		value = nodeData["value"] | false;

		if (value)
			debugOutln('Value updated to true');
		else
			debugOutln('Value updated to false');
	}

	void NodeBool::connectOutport(const std::string &portID, Connection<bool> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<bool>>(out);
	}
} // namespace Node
