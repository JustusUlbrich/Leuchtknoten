#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <ArduinoJson.h>

namespace Node {
    class INode;

    class NodeFactory
    {
    private:
        JsonObject jsonNodes;
    public:
        std::unordered_map<std::string, std::shared_ptr<INode>> nodes;

        explicit NodeFactory(const JsonObject &jsonNodes);
        ~NodeFactory();

        std::shared_ptr<INode> createNodeById(std::string id);
    };
}