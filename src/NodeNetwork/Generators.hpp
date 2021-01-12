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
	// void from_json(const JsonObject &j, DataRgb &x);
	// void to_json(JsonObject &j, const DataRgb &x);

	// void from_json(const JsonObject &j, NodeData &x);
	// void to_json(JsonObject &j, const NodeData &x);

	// void from_json(const JsonObject &j, Connection &x);
	// void to_json(JsonObject &j, const Connection &x);

	// void from_json(const JsonObject &j, Input &x);
	// void to_json(JsonObject &j, const Input &x);

	// void from_json(const JsonObject &j, Output &x);
	// void to_json(JsonObject &j, const Output &x);

	// void from_json(const JsonObject &j, Node &x);
	// void to_json(JsonObject &j, const Node &x);

	// void from_json(const JsonObject &j, NodeNetwork &x);
	// void to_json(JsonObject &j, const NodeNetwork &x);

	// inline void from_json(const JsonObject &j, DataRgb &x)
	// {
	// 	x.r = j["r"].as<int>();
	// 	x.g = j["g"].as<int>();
	// 	x.b = j["b"].as<int>();
	// }

	// inline void to_json(JsonObject &j, const DataRgb &x)
	// {
	// 	j["r"] = x.r;
	// 	j["g"] = x.g;
	// 	j["b"] = x.b;
	// }

	// inline void from_json(const JsonObject &j, NodeData &x)
	// {
	// 	x.value =
	// 		x.num = (j["num"].isNull()) ? std::shared_ptr<int>()
	// 									: std::make_shared<int>(j["num"]);

	// 	if (j["rgb"].isNull())
	// 	{
	// 		x.rgb = nullptr;
	// 	}
	// 	else
	// 	{
	// 		DataRgb rgb;
	// 		from_json(j["rgb"], rgb);
	// 		x.rgb = std::make_shared<DataRgb>(rgb);
	// 	}
	// }

	// inline void to_json(JsonObject &j, const NodeData &x)
	// {
	// 	if (x.num.get() != nullptr)
	// 		j["num"] = *x.num;

	// 	if (x.rgb.get() != nullptr)
	// 	{
	// 		JsonObject rgb;
	// 		to_json(rgb, *x.rgb);
	// 		j["rgb"] = rgb;
	// 	}
	// }

	// inline void from_json(const JsonObject &j, Connection &x)
	// {
	// 	// from_json(j["data"].as<JsonObject>(), x.data);
	// 	x.node = j["node"];
	// 	x.output = j["output"].as<std::string>();
	// }

	// inline void to_json(JsonObject &j, const Connection &x)
	// {
	// 	// JsonObject data;
	// 	// to_json(data, x.data);
	// 	// j["data"] = data;

	// 	j["node"] = x.node;
	// 	j["output"] = x.output;
	// }

	// inline void from_json(const JsonObject &j, ConnectionData &x)
	// {
	// 	// TODO
	// }

	// inline void to_json(JsonObject &j, const ConnectionData &x)
	// {
	// 	// TODO
	// }

	// inline void from_json(const JsonObject &j, std::map<std::string, Input> &x)
	// {
	// 	for (JsonPair ip : j)
	// 	{
	// 		Input input;
	// 		from_json(ip.value().as<JsonObject>(), input);

	// 		x[std::string(ip.key().c_str())] = input;
	// 	}
	// }

	// inline void to_json(JsonObject &j, const std::map<std::string, Input> &x)
	// {
	// 	// TODO
	// }

	// inline void from_json(const JsonObject &j, std::map<std::string, Output> &x)
	// {
	// 	for (JsonPair op : j)
	// 	{
	// 		Output output;
	// 		from_json(op.value().as<JsonObject>(), output);

	// 		x[std::string(op.key().c_str())] = output;
	// 	}
	// }

	// inline void to_json(JsonObject &j, const std::map<std::string, Output> &x)
	// {
	// 	// TODO
	// }

	// inline void from_json(const JsonObject &j, Input &x)
	// {
	// 	for (JsonVariant c : j["connections"].as<JsonArray>())
	// 	{
	// 		Connection con;
	// 		from_json(c, con);
	// 		x.connections.push_back(con);
	// 	}
	// }

	// inline void to_json(JsonObject &j, const Input &x)
	// {
	// 	// j = json::object();
	// 	// j["connections"] = x.connections;
	// }

	// inline void from_json(const JsonObject &j, Output &x)
	// {
	// 	auto connections = j["connections"].as<JsonArray>();

	// 	// Only use the first connection to output port
	// 	if (connections.size() > 0)
	// 	{
	// 		Connection con;
	// 		from_json(connections[0], con);
	// 		x.connection = std::make_shared<Connection>(con);
	// 	}
	// }

	// inline void to_json(JsonObject &j, const Output &x)
	// {
	// 	// j = json::object();
	// 	// j["connections"] = x.connections;
	// }

	// inline void from_json(const JsonObject &j, Node &x)
	// {
	// 	x.id = j["id"].as<int>();
	// 	from_json(j["data"], x.data);
	// 	from_json(j["inputs"], x.inputs);
	// 	from_json(j["outputs"], x.outputs);

	// 	x.position.push_back(j["position"][0]);
	// 	x.position.push_back(j["position"][1]);

	// 	x.name = j["name"].as<std::string>();
	// }

	// inline void to_json(JsonObject &j, const Node &x)
	// {
	// 	// j = json::object();
	// 	// j["id"] = x.id;
	// 	// j["data"] = x.data;
	// 	// j["inputs"] = x.inputs;
	// 	// j["outputs"] = x.outputs;
	// 	// j["position"] = x.position;
	// 	// j["name"] = x.name;
	// }

	// inline void from_json(char *json, NodeNetwork &x)
	// {
	// 	DynamicJsonDocument doc(3072);
	// 	deserializeJson(doc, json, DeserializationOption::NestingLimit(20));

	// 	x.id = doc["id"].as<std::string>();

	// 	for (JsonPair nodePair : doc["nodes"].as<JsonObject>())
	// 	{
	// 		Node node;
	// 		from_json(nodePair.value().as<JsonObject>(), node);

	// 		x.nodes[node.id] = std::make_shared<Node>(node);
	// 	}
	// }

	// inline void to_json(JsonObject &j, const NodeNetwork &x)
	// {
	// 	// j = json::object();
	// 	// j["id"] = x.id;
	// 	// j["nodes"] = x.nodes;
	// }

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

				// out->
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

		// // Check for connection on root input
		// auto connections = rootObject["inputs"]["rgb"]["connections"].as<JsonArray>();
		// if (connections.isNull() == false && connections.size() > 0)
		// {
		// 	auto connected = connections[0]["node"].as<std::string>();
		// }
	}
} // namespace Node
