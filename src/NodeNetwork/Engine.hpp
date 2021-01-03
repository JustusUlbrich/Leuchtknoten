#include "DataRgb.hpp"
#include "NodeNetwork.hpp"

namespace Node {
    DataRgb evaluate(const NodeNetwork& network) {

        auto it = network.nodes.begin();
        while (it != network.nodes.end())
        {
            if (it->second.name == "Output")
                break;

            it++;
        }

        auto rgbOut = it->second.inputs.at("rgb");

        DataRgb rgb;
        if (rgbOut.connections.size() > 0)
        {
            char buf[10];
            auto nodeId = std::string(itoa(rgbOut.connections[0].node, buf, 10));
            rgb = *network.nodes.at(nodeId).data.rgb;
        }

        return rgb;
    }
}