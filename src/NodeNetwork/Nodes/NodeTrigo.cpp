#include "NodeTrigo.hpp"
#include <math.h>

namespace Node
{
	NodeTrigo::NodeTrigo(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		in = std::make_shared<InputPort<float>>("in", this);
		sinOut = std::make_shared<OutputPort<float>>("sin", this);
		cosOut = std::make_shared<OutputPort<float>>("cos", this);
		tanOut = std::make_shared<OutputPort<float>>("tan", this);

		connectInport(nodeJson, nodeFactory, in, "in");
	}

	NodeTrigo::~NodeTrigo()
	{
	}

	void NodeTrigo::eval(const Context &context, const LedContext &ledContext, const std::string &portId, float &out)
	{
		float value = 0.f;

		if (in->connection.has_value())
		{
			auto con = in->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}

		if (portId == "sin")
			out = sinf(value);
		else if(portId == "cos")
			out = cosf(value);
		else if(portId == "tan")
			out = tanf(value);
	}

	void NodeTrigo::connectOutport(const std::string &portID, Connection<float> &connection)
	{
		if (portID == "sin")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(sinOut);
		else if(portID == "cos")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(cosOut);
		else if(portID == "tan")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(tanOut);
	}
} // namespace Node
