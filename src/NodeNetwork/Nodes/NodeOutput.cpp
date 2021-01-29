#include "NodeOutput.hpp"
#include "../NodeFactory.hpp"

#include "../../defines.h"

namespace Node
{

	NodeOutput::NodeOutput(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		in = std::make_shared<InputPort<CRGB>>("rgb", this);

		connectInport(nodeJson, nodeFactory, in, "rgb");
	}

	NodeOutput::~NodeOutput()
	{
	}

	CRGB NodeOutput::eval(const Context &context, const LedContext &ledContext)
	{
		debugOut("\t\t eval at node: ");
		debugOutln(name.c_str());

		if (in->connection)
		{
			debugOut("\t\t\t eval connection");

			auto con = *in->connection;
			if (con.fromPort != nullptr)
				return con.fromPort->eval(context, ledContext);
			else
				debugOut("\t\t\t fromPort empty :(");

			return CRGB::Black;
		}
	}
} // namespace Node
