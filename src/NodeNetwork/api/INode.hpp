#pragma once

#include <memory>
#include <vector>

#include <ArduinoJson.h>

#include "Connection.hpp"
#include "DataRgb.hpp"
#include "../../context.hpp"

// #include "NodeData.hpp"

namespace Node
{
	class NodeFactory;
	class INode
	{
	public:
		// TODO: private?
		int id = -1;
		// NodeData data;
		std::vector<int> position;
		std::string name = "";

		explicit INode(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory) {};

		// TODO: Think about refactoring this

		virtual void eval(const Context &context, const LedContext &ledContext, DataRgb &out){};
		virtual void eval(const Context &context, const LedContext &ledContext, int &out){};
		virtual void eval(const Context &context, const LedContext &ledContext, float &out){};

		virtual void connectOutport(const std::string &portID, Connection<DataRgb> &connection){};
		virtual void connectOutport(const std::string &portID, Connection<int> &connection){};
		virtual void connectOutport(const std::string &portID, Connection<float> &connection){};
	};

} // namespace Node