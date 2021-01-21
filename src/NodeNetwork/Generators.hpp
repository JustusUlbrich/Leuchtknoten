#pragma once

#include <unordered_map>

#include <ArduinoJson.h>

#include "../defines.h"

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
#include "nodes/NodeLookup.hpp"
#include "nodes/NodeTrigo.hpp"
#include "nodes/NodeMath.hpp"

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
		debugOut("\t Start Node: ");
		debugOutln(nodeID.c_str());

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
			newNode = std::make_shared<NodeGradient>(nodeJson, nodeFactory);
		}
		else if (nodeJson["name"] == "Output")
		{
			newNode = std::make_shared<NodeOutput>(nodeJson, nodeFactory);
		}
		else if (nodeJson["name"] == "Lookup")
		{
			newNode = std::make_shared<NodeLookup>(nodeJson, nodeFactory);
		}
		else if (nodeJson["name"] == "Trigo")
		{
			newNode = std::make_shared<NodeTrigo>(nodeJson, nodeFactory);
		}
		else if (nodeJson["name"] == "Math")
		{
			newNode = std::make_shared<NodeMath>(nodeJson, nodeFactory);
		}
		else
		{
			debugOut("Unknown Node: ");
			debugOutln(jsonNodes[nodeID].as<std::string>().c_str());
		}

		if (newNode == nullptr)
			return nullptr;

		fillNodeFromJson(nodeJson, newNode);

		debugOut("\t End Node:");
		debugOutln(nodeID.c_str());
		debugOut("\t name:");
		debugOutln(newNode->name.c_str());

		return newNode;
	}

	static std::unordered_map<std::string, std::shared_ptr<INode>> fromNetworkJson(JsonObject &doc, std::string &rootId)
	{
		JsonObject jsonNodes = doc["nodes"].as<JsonObject>();

		debugOutln(doc["nodes"].as<std::string>().c_str());

		JsonObject rootObject;

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

	static std::unordered_map<std::string, std::shared_ptr<INode>> fromNetworkJson(char *json, std::string &rootId)
	{
		debugOutln("Start parse");

		DynamicJsonDocument doc(8192);
		deserializeJson(doc, json, DeserializationOption::NestingLimit(50));

		JsonObject jsonObj = doc.as<JsonObject>();

		return fromNetworkJson(jsonObj, rootId);
	}
} // namespace Node
