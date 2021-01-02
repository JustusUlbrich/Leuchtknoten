#pragma once

#include "json.hpp"
#include "helper.hpp"

#include "NodeNetwork.hpp"
#include "Node.hpp"
#include "Outputs.hpp"
#include "Inputs.hpp"
#include "ConnectionData.hpp"
#include "NodeData.hpp"
#include "DataRgb.hpp"

namespace nlohmann
{
	void from_json(const json &j, Node::DataRgb &x);
	void to_json(json &j, const Node::DataRgb &x);

	void from_json(const json &j, Node::NodeData &x);
	void to_json(json &j, const Node::NodeData &x);

	void from_json(const json &j, Node::Connection &x);
	void to_json(json &j, const Node::Connection &x);

	void from_json(const json &j, Node::Input &x);
	void to_json(json &j, const Node::Input &x);

	void from_json(const json &j, Node::Output &x);
	void to_json(json &j, const Node::Output &x);

	void from_json(const json &j, Node::Node &x);
	void to_json(json &j, const Node::Node &x);

	void from_json(const json &j, Node::NodeNetwork &x);
	void to_json(json &j, const Node::NodeNetwork &x);

	inline void from_json(const json &j, Node::DataRgb &x)
	{
		x.r = j.at("r").get<int>();
		x.g = j.at("g").get<int>();
		x.b = j.at("b").get<int>();
	}

	inline void to_json(json &j, const Node::DataRgb &x)
	{
		j = json::object();
		j["r"] = x.r;
		j["g"] = x.g;
		j["b"] = x.b;
	}

	inline void from_json(const json &j, Node::NodeData &x)
	{
		x.num = Node::get_optional<int>(j, "num");
		x.rgb = Node::get_optional<Node::DataRgb>(j, "rgb");
	}

	inline void to_json(json &j, const Node::NodeData &x)
	{
		j = json::object();
		j["num"] = x.num;
		j["rgb"] = x.rgb;
	}

	inline void from_json(const json &j, Node::Connection &x)
	{
		x.data = j.at("data").get<Node::ConnectionData>();
		x.node = j.at("node").get<int>();
		x.output = j.at("output").get<std::string>();
	}

	inline void to_json(json &j, const Node::Connection &x)
	{
		j = json::object();
		j["data"] = x.data;
		j["node"] = x.node;
		j["output"] = x.output;
	}

	inline void from_json(const json &j, Node::ConnectionData &x)
	{
		// TODO
	}

	inline void to_json(json &j, const Node::ConnectionData &x)
	{
		// TODO
		j = json::object();
	}

	inline void from_json(const json &j, Node::Input &x)
	{
		x.connections = j.at("connections").get<std::vector<Node::Connection>>();
	}

	inline void to_json(json &j, const Node::Input &x)
	{
		j = json::object();
		j["connections"] = x.connections;
	}

	inline void from_json(const json &j, Node::Output &x)
	{
		x.connections = j.at("connections").get<std::vector<Node::Connection>>();
	}

	inline void to_json(json &j, const Node::Output &x)
	{
		j = json::object();
		j["connections"] = x.connections;
	}

	inline void from_json(const json &j, Node::Node &x)
	{
		x.id = j.at("id").get<int>();
		x.data = j.at("data").get<Node::NodeData>();
		x.inputs = j.at("inputs").get<std::map<std::string, Node::Input>>();
		x.outputs = j.at("outputs").get<std::map<std::string, Node::Output>>();
		x.position = j.at("position").get<std::vector<int>>();
		x.name = j.at("name").get<std::string>();
	}

	inline void to_json(json &j, const Node::Node &x)
	{
		j = json::object();
		j["id"] = x.id;
		j["data"] = x.data;
		j["inputs"] = x.inputs;
		j["outputs"] = x.outputs;
		j["position"] = x.position;
		j["name"] = x.name;
	}

	inline void from_json(const json &j, Node::NodeNetwork &x)
	{
		x.id = j.at("id").get<std::string>();
		x.nodes = j.at("nodes").get<std::map<std::string, Node::Node>>();
	}

	inline void to_json(json &j, const Node::NodeNetwork &x)
	{
		j = json::object();
		j["id"] = x.id;
		j["nodes"] = x.nodes;
	}
} // namespace nlohmann
