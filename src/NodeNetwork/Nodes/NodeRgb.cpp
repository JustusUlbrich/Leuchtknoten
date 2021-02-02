#include "NodeRgb.hpp"

#include "../../defines.h"

namespace Node
{
	NodeRgb::NodeRgb(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		out = std::make_shared<OutputPort<CRGB>>(
			"num",
			this,
			[this](const Context &c, const LedContext &lc) { return value; });

		if (nodeJson["data"]["rgb"])
		{
			value.r = nodeJson["data"]["rgb"]["r"] | 0;
			value.g = nodeJson["data"]["rgb"]["g"] | 0;
			value.b = nodeJson["data"]["rgb"]["b"] | 0;
		}
	}

	NodeRgb::~NodeRgb()
	{
	}

	void NodeRgb::updateValue(const ArduinoJson::JsonObject &nodeData)
	{
		if (nodeData["rgb"])
		{
			value.r = nodeData["rgb"]["r"] | 0;
			value.g = nodeData["rgb"]["g"] | 0;
			value.b = nodeData["rgb"]["b"] | 0;
		}
	}

	void NodeRgb::connectOutport(const std::string &portID, Connection<CRGB> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<CRGB>>(out);
	}
} // namespace Node
