#include "NodeNumber.hpp"

namespace Node
{
	NodeNumber::NodeNumber(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		out = std::make_shared<OutputPort<float>>(
			"num",
			this,
			[this](const Context &c, const LedContext &lc) { return value; });

		value = nodeJson["data"]["num"] | 0.f;
	}

	NodeNumber::~NodeNumber()
	{
	}

	void NodeNumber::updateValue(const ArduinoJson::JsonObject &nodeData)
	{
		value = nodeData["num"] | 0.f;
	}

	void NodeNumber::connectOutport(const std::string &portID, Connection<float> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<float>>(out);
	}
} // namespace Node
