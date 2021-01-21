#include "NodeTrigo.hpp"
#include <math.h>

namespace Node
{
	NodeTrigo::NodeTrigo(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		in = std::make_shared<InputPort<float>>("in", this);
		out = std::make_shared<OutputPort<float>>("out", this);

		mode = nodeJson["data"]["mode"] | 0;
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

		if (mode == 0)
			out = sinf(value);
	}

	void NodeTrigo::connectOutport(const std::string &portID, Connection<float> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<float>>(out);
	}
} // namespace Node
