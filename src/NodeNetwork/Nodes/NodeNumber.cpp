#include "NodeNumber.hpp"

namespace Node
{
	NodeNumber::NodeNumber(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		out = std::make_shared<OutputPort<int>>("num", this);

		value = nodeJson["data"]["num"] | 0;
	}

	NodeNumber::~NodeNumber()
	{
	}

	void NodeNumber::eval(const Context &context, const LedContext &ledContext, int &out)
	{
		out = value;
	}

	void NodeNumber::connectOutport(const std::string &portID, Connection<int> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<int>>(out);
	}
} // namespace Node
