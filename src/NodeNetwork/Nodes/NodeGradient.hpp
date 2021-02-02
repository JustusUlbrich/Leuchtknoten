#pragma once
#include <string>

#include "../api/DataGradient.hpp"
#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeGradient : public INode
	{

	public:
		explicit NodeGradient(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory);
		~NodeGradient();

		CRGB evalRgb(const Context &context, const LedContext &ledContext);

		void updateValue(const ArduinoJson::JsonObject &nodeJson) override;
		void connectOutport(const std::string &portID, Connection<CRGB> &connection) override;

		std::shared_ptr<InputPort<float>> scaleIn;
		std::shared_ptr<OutputPort<CRGB>> out;
		DataGradient gradient;
	};

} // namespace Node