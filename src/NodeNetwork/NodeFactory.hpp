#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../config.hpp"
#include <ArduinoJson.h>

namespace Node {
	class INode;

	class NodeFactory
	{
	private:
		JsonObject jsonNodes;
		Config config;

	public:
		std::unordered_map<std::string, std::shared_ptr<INode>> nodes;

		explicit NodeFactory(const JsonObject& jsonNodes, const Config& config);
		~NodeFactory();

		Config& getConfig();

		std::shared_ptr<INode> createNodeById(std::string id);
	};
}