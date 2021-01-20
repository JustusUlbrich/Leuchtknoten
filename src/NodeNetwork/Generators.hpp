#pragma once

#include <unordered_map>

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

#include "NodeFactory.hpp"

namespace Node
{
	static void fillNodeFromJson(JsonObject &j, std::shared_ptr<INode> node)
	{
		node->id = j["id"].as<int>();
		node->name = j["name"].as<std::string>();

		// node->position.push_back(j["position"][0].as<int>());
		// node->position.push_back(j["position"][1].as<int>());
	}

	static std::shared_ptr<INode> createNodeFromJson(const std::string &nodeID, JsonObject &jsonNodes, NodeFactory *nodeFactory)
	{
		Serial.print("\t Start Node:");
		Serial.println(nodeID.c_str());

		// auto createChild = [&jsonNodes, &nodes](const std::string& childID) {
		// 	createNodeFromJson(childID, jsonNodes, nodes);
		// };

		std::shared_ptr<INode> newNode = nullptr;

		auto nodeJson = jsonNodes[nodeID].as<JsonObject>();
		// TODO: Better handling of node types and creation eg. factory etc
		// Probably just put it into constructor of nodes and pass json
		if (nodeJson["name"] == "RGB")
		{
			newNode = std::make_shared<NodeRgb>(nodeJson, nodeFactory);
		}
		else if (nodeJson["name"] == "Number")
		{
			newNode = std::make_shared<NodeNumber>(nodeJson, nodeFactory);
		}
		else if (nodeJson["name"] == "Gradient")
		{
			auto gradientNode = std::make_shared<NodeGradient>(nodeJson, nodeFactory);

			auto jsonGradients = nodeJson["data"]["gradient"].as<JsonArray>();
			for (JsonVariant v : jsonGradients)
			{
				Node::GradientEntry e;
				e.offset = v["offset"].as<float>();
				e.color = DataRgb::fromHex(v["color"].as<std::string>());

				gradientNode->gradient.entries.push_back(e);
			}

			// TODO: Check for connections and handle

			// nodes.push_back(gradientNode);
		}
		else if (nodeJson["name"] == "Output")
		{
			newNode = std::make_shared<NodeOutput>(nodeJson, nodeFactory);
		}

		if (newNode == nullptr)
			return nullptr;

		fillNodeFromJson(nodeJson, newNode);

		Serial.print("\t End Node:");
		Serial.println(nodeID.c_str());
		Serial.print("\t name:");
		Serial.println(newNode->name.c_str());

		return newNode;
	}

	static std::unordered_map<std::string, std::shared_ptr<INode>> fromNetworkJson(char *json)
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
			return std::unordered_map<std::string, std::shared_ptr<INode>>();

		NodeFactory factory{jsonNodes};
		factory.createNodeById(rootId);

		return factory.nodes;
	}
} // namespace Node
