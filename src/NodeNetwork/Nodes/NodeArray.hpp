#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeArray : public INode
	{

	public:
		explicit NodeArray(const ArduinoJson::JsonObject& nodeJson, NodeFactory* nodeFactory);
		~NodeArray();

		float evalOut(const Context& context, const LedContext& ledContext);

		void connectOutport(const std::string& portID, Connection<float>& connection) override;
		void updateValue(const ArduinoJson::JsonObject& nodeData);

		std::shared_ptr<InputPort<float>> index;
		std::shared_ptr<OutputPort<float>> out;

		std::vector<float> entries;
	};

} // namespace Node