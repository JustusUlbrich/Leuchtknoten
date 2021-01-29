#pragma once
#include <string>

#include "../api/DataGradient.hpp"
#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeMix : public INode
	{

	public:
		explicit NodeMix(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory);
		~NodeMix();

		CRGB evalRgb(const Context &context, const LedContext &ledContext);
		void connectOutport(const std::string &portID, Connection<CRGB> &connection) override;

		std::shared_ptr<InputPort<CRGB>> c1In;
		std::shared_ptr<InputPort<CRGB>> c2In;
		std::shared_ptr<InputPort<float>> scaleIn;
		std::shared_ptr<OutputPort<CRGB>> mixOut;
		DataGradient gradient;
	};

} // namespace Node