#pragma once

#include <ArduinoJson.h>

#include "api/Port.hpp"
#include "api/Connection.hpp"
#include "api/ConnectionData.hpp"
#include "api/NodeData.hpp"
#include "api/DataRgb.hpp"
#include "api/DataGradient.hpp"

#include "nodes/NodeTypes.hpp"
#include "nodes/NodeNumber.hpp"
#include "nodes/NodeRgb.hpp"
#include "nodes/NodeGradient.hpp"
#include "nodes/NodeOutput.hpp"

namespace Node
{
	void fillNodeFromJson(JsonObject &j, std::shared_ptr<INode> node)
	{
		node->id = j["id"].as<int>();
		node->name = j["name"].as<std::string>();

		// node->position.push_back(j["position"][0].as<int>());
		// node->position.push_back(j["position"][1].as<int>());
	}

	void createNodeFromJson(std::string &nodeID, JsonObject &jsonNodes, std::vector<std::shared_ptr<INode>> &nodes)
	{
		Serial.print("\t Start Node:");
		Serial.println(nodeID.c_str());

		auto nodeJson = jsonNodes[nodeID].as<JsonObject>();
		// TODO: Better handling of node types and creation eg. factory etc
		// Probably just put it into constructor of nodes and pass json
		if (nodeJson["name"] == "RGB")
		{
			auto rgb = std::make_shared<NodeRgb>();
			rgb->value.r = nodeJson["data"]["rgb"]["r"];
			rgb->value.g = nodeJson["data"]["rgb"]["g"];
			rgb->value.b = nodeJson["data"]["rgb"]["b"];

			nodes.push_back(rgb);
		}
		else if (nodeJson["name"] == "Number")
		{
			auto num = std::make_shared<NodeNumber>();
			num->value = nodeJson["data"]["num"].as<int>();

			nodes.push_back(num);
		}
		else if (nodeJson["name"] == "Gradient")
		{
			auto gradientNode = std::make_shared<NodeGradient>();

			auto jsonGradients = nodeJson["data"]["gradient"].as<JsonArray>();
			for (JsonVariant v : jsonGradients)
			{
				Node::GradientEntry e;
				e.offset = v["offset"].as<float>();
				e.color = DataRgb::fromHex(v["color"].as<std::string>());

				gradientNode->gradient.entries.push_back(e);
			}

			// TODO: Check for connections and handle

			nodes.push_back(gradientNode);
		}
		else if (nodeJson["name"] == "Output")
		{
			auto outNode = std::make_shared<NodeOutput>();

			// Check for connections and handle
			auto connections = nodeJson["inputs"]["rgb"]["connections"].as<JsonArray>();
			if (connections.isNull() == false && connections.size() > 0)
			{
				auto connectedId = connections[0]["node"].as<std::string>();
				createNodeFromJson(connectedId, jsonNodes, nodes);

				Serial.print("\t\t\t Created Node name:");
				Serial.println(nodes[nodes.size() - 1]->name.c_str());

				auto con = Connection<DataRgb>();
				con.toPort = outNode->in;

				nodes[nodes.size() - 1]->connectOutport("rgb", con);

				outNode->in->connection = nonstd::optional<Connection<DataRgb>>(con);
			}

			nodes.push_back(outNode);
		}

		fillNodeFromJson(nodeJson, nodes[nodes.size() - 1]);

		Serial.print("\t End Node:");
		Serial.println(nodeID.c_str());
		Serial.print("\t name:");
		Serial.println(nodes[nodes.size() - 1]->name.c_str());
	}

	void fromNetworkJson(char *json, std::vector<std::shared_ptr<INode>> &nodes)
	{
		Serial.println("Start parse");

		DynamicJsonDocument doc(3072);
		deserializeJson(doc, json, DeserializationOption::NestingLimit(20));

		// TODO: Node string to types ?

		JsonObject jsonNodes = doc["nodes"].as<JsonObject>();

		JsonObject rootObject;
		std::string rootId;

		// Search for output node
		for (JsonPair nodePair : doc["nodes"].as<JsonObject>())
		{
			if (nodePair.value()["name"] == "Output")
			{
				rootObject = nodePair.value();
				rootId = std::string(nodePair.key().c_str());
				break;
			}
		}

		if (rootObject.isNull())
			return;

		createNodeFromJson(rootId, jsonNodes, nodes);
	}
} // namespace Node
