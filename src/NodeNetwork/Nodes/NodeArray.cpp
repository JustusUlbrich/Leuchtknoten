#include "NodeArray.hpp"
#include "../NodeFactory.hpp"

namespace Node
{
	NodeArray::NodeArray(const ArduinoJson::JsonObject& nodeJson, NodeFactory* nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		index = std::make_shared<InputPort<float>>("index", this);
		out = std::make_shared<OutputPort<float>>(
			"value",
			this,
			[this](const Context& c, const LedContext& lc) { return evalOut(c, lc); });

		auto jsonArray = nodeJson["data"]["entries"].as<JsonArray>();
		for (JsonVariant v : jsonArray)
		{
			entries.push_back(v | 0.f);
		}

		connectInport(nodeJson, nodeFactory, index, "index");
	}

	NodeArray::~NodeArray()
	{
	}

	void NodeArray::updateValue(const ArduinoJson::JsonObject& nodeData)
	{
		entries.clear();

		auto jsonArray = nodeData["entries"].as<JsonArray>();
		for (JsonVariant v : jsonArray)
		{
			entries.push_back(v | 0.f);
		}
	}

	float NodeArray::evalOut(const Context& context, const LedContext& ledContext)
	{
		if (entries.size() == 0)
			return 0.f;

		int arrayIndex = ledContext.id;

		if (index->connection.has_value())
		{
			auto con = index->connection.value();
			arrayIndex = (int)con.fromPort->eval(context, ledContext);
		}

		arrayIndex = arrayIndex % entries.size();
		arrayIndex = arrayIndex < 0 ? arrayIndex + entries.size() : arrayIndex;
		return entries[arrayIndex];
	}

	void NodeArray::connectOutport(const std::string& portID, Connection<float>& connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<float>>(out);
	}

} // namespace Node