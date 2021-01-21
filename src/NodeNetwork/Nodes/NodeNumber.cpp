#include "NodeNumber.hpp"

namespace Node
{
	NodeNumber::NodeNumber(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		out = std::make_shared<OutputPort<float>>("num", this);

		value = nodeJson["data"]["num"] | 0.f;
	}

	NodeNumber::~NodeNumber()
	{
	}

	void NodeNumber::eval(const Context &context, const LedContext &ledContext, const std::string &portId, float &out)
	{
		out = value;
	}

	void NodeNumber::connectOutport(const std::string &portID, Connection<float> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<float>>(out);
	}
} // namespace Node
