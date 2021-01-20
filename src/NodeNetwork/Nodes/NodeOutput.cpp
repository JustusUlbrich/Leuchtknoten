#include "NodeOutput.hpp"
#include "../NodeFactory.hpp"

namespace Node
{


	NodeOutput::NodeOutput(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		in = std::make_shared<InputPort<DataRgb>>("rgb", this);

		// Check for connections and handle
			auto connections = nodeJson["inputs"]["rgb"]["connections"].as<JsonArray>();
			if (connections.isNull() == false && connections.size() > 0)
			{
				auto connectedId = connections[0]["node"].as<std::string>();
				auto connectedNode = nodeFactory->createNodeById(connectedId);

				Serial.print("\t\t\t Created Node name:");
				Serial.println(connectedNode->name.c_str());

				auto con = Connection<DataRgb>();
				con.toPort = this->in;

				connectedNode->connectOutport("rgb", con);

				this->in->connection = nonstd::optional<Connection<DataRgb>>(con);
			}
	}

	NodeOutput::~NodeOutput()
	{
	}

	void NodeOutput::eval(const Context &context, const LedContext &ledContext, DataRgb &out)
	{
		Serial.print("\t\t eval at node: ");
		Serial.println(name.c_str());

		if (in->connection)
		{
			Serial.print("\t\t\t eval connection");

			auto con = *in->connection;
			if (con.fromPort != nullptr)
				out = con.fromPort->eval(context, ledContext);
			else
				Serial.print("\t\t\t fromPort empty :(");
		}
		else
		{
			Serial.print("\t\t\t connection empty :(");
		}

		Serial.println("\t\t done eval output");
	}
} // namespace Node
