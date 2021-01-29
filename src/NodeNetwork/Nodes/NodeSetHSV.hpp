#pragma once
#include <string>

#include "../api/DataGradient.hpp"
#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeSetHSV : public INode
	{

	public:
		explicit NodeSetHSV(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory);
		~NodeSetHSV();

		CRGB evalRgb(const Context &context, const LedContext &ledContext);
		void connectOutport(const std::string &portID, Connection<CRGB> &connection) override;

		std::shared_ptr<InputPort<CRGB>> colorIn;
		std::shared_ptr<InputPort<float>> hIn;
		std::shared_ptr<InputPort<float>> sIn;
		std::shared_ptr<InputPort<float>> vIn;
		std::shared_ptr<OutputPort<CRGB>> rgbOut;
		DataGradient gradient;
	};

} // namespace Node