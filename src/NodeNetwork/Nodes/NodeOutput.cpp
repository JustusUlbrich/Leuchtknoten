#include "NodeOutput.hpp"
#include "../NodeFactory.hpp"

#include "../../defines.h"

namespace Node
{

	NodeOutput::NodeOutput(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		in = std::make_shared<InputPort<DataRgb>>("rgb", this);

		connectInport(nodeJson, nodeFactory, in, "rgb");
	}

	NodeOutput::~NodeOutput()
	{
	}

	void NodeOutput::eval(const Context &context, const LedContext &ledContext, DataRgb &out)
	{
		debugOut("\t\t eval at node: ");
		debugOutln(name.c_str());

		if (in->connection)
		{
			debugOut("\t\t\t eval connection");

			auto con = *in->connection;
			if (con.fromPort != nullptr)
				out = con.fromPort->eval(context, ledContext);
			else
				debugOut("\t\t\t fromPort empty :(");
		}
	}
} // namespace Node
