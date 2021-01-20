#include "NodeFactory.hpp"

#include "api/INode.hpp"
#include "Generators.hpp"

namespace Node {
    NodeFactory::NodeFactory(const JsonObject &jsonNodes)
        : jsonNodes(jsonNodes)
    {
    }

    NodeFactory::~NodeFactory()
    {
    }

    std::shared_ptr<INode> NodeFactory::createNodeById(std::string id)
    {
        auto search = nodes.find(id);
        if (search != nodes.end())
            return search->second;

        auto node = createNodeFromJson(id, jsonNodes, this);
        nodes[id] = node;

        return node;
    }
}