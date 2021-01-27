#include "NodeLookup.hpp"

#include "../../defines.h"

namespace Node
{
	NodeLookup::NodeLookup(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		nodeIdOut = std::make_shared<OutputPort<float>>(
			"nodeId",
			 this,
			[this](const Context &c, const LedContext &lc) { return evalNodeId(c,lc); }
		);
		numLedsOut = std::make_shared<OutputPort<float>>(
			"numLeds",
			this,
			[this](const Context &c, const LedContext &lc) { return evalNumLeds(c,lc); }
		);
		elapsedOut = std::make_shared<OutputPort<float>>(
			"elapsed",
			this,
			[this](const Context &c, const LedContext &lc) { return evalElapsed(c,lc); }
		);
	}

	NodeLookup::~NodeLookup()
	{
	}


	float NodeLookup::evalNodeId(const Context &context, const LedContext &ledContext)
	{
		return ledContext.id;
	}

	float NodeLookup::evalNumLeds(const Context &context, const LedContext &ledContext)
	{
		return context.numLeds;
	}

	float NodeLookup::evalElapsed(const Context &context, const LedContext &ledContext)
	{
		return context.elapsed / 1000.f;
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
