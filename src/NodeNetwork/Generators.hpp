#pragma once

#include <ArduinoJson.h>

#include "api/Port.hpp"
#include "api/Connection.hpp"
#include "api/ConnectionData.hpp"
#include "api/NodeData.hpp"
#include "api/DataRgb.hpp"

#include "nodes/NodeTypes.hpp"
#include "nodes/NodeNumber.hpp"
#include "nodes/NodeRgb.hpp"
#include "nodes/NodeOutput.hpp"

namespace Node
{
	void fillNodeFromJson(JsonObject &j, std::shared_ptr<INode> node)
	{
		node->id = j["id"].as<int>();
		node->name = j["name"].as<std::string>();

		node->position.push_back(j["position"][0]);
		node->position.push_back(j["position"][1]);
	}

	std::shared_ptr<INode> createNodeFromJson(std::string &nodeID, JsonObject &nodes)
	{
		std::shared_ptr<INode> node;

		auto nodeJson = nodes[nodeID].as<JsonObject>();
		// TODO: Better handling of node types and creation eg. factory etc
		// Probably just put it into constructor of nodes and pass json
		if (nodeJson["name"] == "RGB")
		{
			auto rgb = std::make_shared<NodeRgb>();
			rgb->value.r = nodeJson["data"]["rgb"]["r"];
			rgb->value.g = nodeJson["data"]["rgb"]["g"];
			rgb->value.b = nodeJson["data"]["rgb"]["b"];

			node = rgb;
		}
		else if (nodeJson["name"] == "Number")
		{
			auto num = std::make_shared<NodeNumber>();
			num->value = nodeJson["data"]["num"];

			node = num;
		}
		else if (nodeJson["name"] == "Output")
		{
			auto out = std::make_shared<NodeOutput>();

			// Check for connection on output node
			auto connections = nodeJson["inputs"]["rgb"]["connections"].as<JsonArray>();
			if (connections.isNull() == false && connections.size() > 0)
			{
				auto connectedId = connections[0]["node"].as<std::string>();
				auto connectedNode = createNodeFromJson(connectedId, nodes);

				auto con = Connection<DataRgb>();
				con.toPort = out->in;

				// TODO: Get output port from connectedNode and connect it
				// con.fromPort = connectedNode.getOutPortAs<DataRGB>("")
			}
		}

		fillNodeFromJson(nodeJson, node);
		return node;
	}

	std::shared_ptr<INode> fromNetworkJson(char *json)
	{
		DynamicJsonDocument doc(3072);
		deserializeJson(doc, json, DeserializationOption::NestingLimit(20));

		// TODO: Node string to types ?

		JsonObject nodes = doc["nodes"].as<JsonObject>();

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
			return nullptr;

		auto rootNode = createNodeFromJson(rootId, nodes);

		return rootNode;
	}
} // namespace Node
