#include "NodeLookup.hpp"

#include "../../defines.h"

namespace Node
{
	NodeLookup::NodeLookup(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		nodeIdOut = std::make_shared<OutputPort<float>>("nodeId", this);
		numLedsOut = std::make_shared<OutputPort<float>>("numLeds", this);
		elapsedOut = std::make_shared<OutputPort<float>>("elapsed", this);
	}

	NodeLookup::~NodeLookup()
	{
	}

	void NodeLookup::eval(const Context &context, const LedContext &ledContext, const std::string &portId, float &out)
	{
		if (portId == "nodeId")
			out = ledContext.id;
		else if (portId == "numLeds")
			out = context.numLeds;
		else if (portId == "elapsed")
			out = context.elapsed;
	}

	void NodeLookup::connectOutport(const std::string &portID, Connection<float> &connection)
	{
		if (portID == "nodeId")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(nodeIdOut);
		else if (portID == "numLeds")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(numLedsOut);
		else if (portID == "elapsed")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(elapsedOut);
	}
} // namespace Node
