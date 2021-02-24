#pragma once

#include <FastLED.h>
#include <memory>
#include <vector>
#include "optional.hpp"

#include "../../defines.h"

#include <ArduinoJson.h>

#include "Connection.hpp"

#include "../NodeFactory.hpp"
#include "../../context.hpp"
#include "../../defines.h"

namespace Node
{
	class INode
	{
	public:
		// TODO: private?
		int id = -1;
		std::vector<int> position;
		std::string name = "";

		explicit INode(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory){};

		template <typename T>
		void connectInport(
			const ArduinoJson::JsonObject &nodeJson,
			NodeFactory *nodeFactory,
			std::shared_ptr<InputPort<T>> inPort,
			const std::string &inportID)
		{
			// Check for connections and handle
			auto connections = nodeJson["inputs"][inportID]["connections"].as<JsonArray>();
			if (connections.isNull() == false && connections.size() > 0)
			{
				auto connectedId = connections[0]["node"].as<std::string>();
				auto connectedNode = nodeFactory->createNodeById(connectedId);
				if (connectedNode == nullptr)
				{
					// debugOut("Could not create Node");
					// debugOutln(connectedId);
					return;
				}

				auto con = Connection<T>();
				con.toPort = inPort;

				auto connectedPortId = connections[0]["output"].as<std::string>();
				connectedNode->connectOutport(connectedPortId, con);

				inPort->connection = nonstd::optional<Connection<T>>(con);
			}
		}

		virtual void update(const float delta, const Context &context, const LedContext &ledContext) {};

		virtual void updateValue(const ArduinoJson::JsonObject &nodeData){};

		// TODO: Think about refactoring this
		virtual void connectOutport(const std::string &portID, Connection<CRGB> &connection){};
		virtual void connectOutport(const std::string &portID, Connection<float> &connection){};
		virtual void connectOutport(const std::string &portID, Connection<bool> &connection){};
	};

} // namespace Node