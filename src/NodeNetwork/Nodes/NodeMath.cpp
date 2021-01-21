#include "NodeMath.hpp"

#include "../../defines.h"

namespace Node
{
	NodeMath::NodeMath(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		in1 = std::make_shared<InputPort<float>>("in1", this);
		in2 = std::make_shared<InputPort<float>>("in2", this);

		addOut = std::make_shared<OutputPort<float>>("add", this);
		subOut = std::make_shared<OutputPort<float>>("sub", this);
		mulOut = std::make_shared<OutputPort<float>>("mul", this);
		divOut = std::make_shared<OutputPort<float>>("div", this);

		connectInport(nodeJson, nodeFactory, in1, "in1");
		connectInport(nodeJson, nodeFactory, in2, "in2");
	}

	NodeMath::~NodeMath()
	{
	}

	void NodeMath::eval(const Context &context, const LedContext &ledContext, const std::string &portId, float &out)
	{
		out = 0;

		float value1 = 0.f, value2 = 0.f;

		if (in1->connection.has_value())
		{
			auto con = in1->connection.value();
			value1 = con.fromPort->eval(context, ledContext);
		}

		if (in2->connection.has_value())
		{
			auto con = in2->connection.value();
			value2 = con.fromPort->eval(context, ledContext);
		}

		if (portId == "add")
			out = value1 + value2;
		else if (portId == "sub")
			out = value1 - value2;
		else if (portId == "mul")
			out = value1 * value2;
		else if (portId == "div" && value2 != 0.f)
			out = value1 / value2;
	}

	void NodeMath::connectOutport(const std::string &portID, Connection<float> &connection)
	{
		if (portID == "add")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(addOut);
		else if (portID == "sub")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(subOut);
		else if (portID == "mul")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(mulOut);
		else if (portID == "div")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(divOut);
	}
} // namespace Node
