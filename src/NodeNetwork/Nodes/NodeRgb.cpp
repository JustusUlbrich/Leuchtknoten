#include "NodeRgb.hpp"

#include "../../defines.h"

namespace Node
{
	NodeRgb::NodeRgb(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		out = std::make_shared<OutputPort<DataRgb>>("num", this);

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

	void NodeRgb::eval(const Context &context, const LedContext &ledContext, const std::string &portId, DataRgb &out)
	{
		debugOut("\t\t eval rgb");
		debugOutln(id);

		out = value;
	}

	void NodeRgb::connectOutport(const std::string &portID, Connection<DataRgb> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<DataRgb>>(out);
	}
} // namespace Node
