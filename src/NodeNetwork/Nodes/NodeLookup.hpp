#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeLookup : public INode
	{

	public:
		explicit NodeLookup(const ArduinoJson::JsonObject& nodeJson, NodeFactory* nodeFactory);
		~NodeLookup();

		void connectOutport(const std::string& portID, Connection<float>& connection) override;

		float evalNodeId(const Context& context, const LedContext& ledContext);
		float evalNumLeds(const Context& context, const LedContext& ledContext);
		float evalElapsed(const Context& context, const LedContext& ledContext);
		float evalPosX(const Context& context, const LedContext& ledContext);
		float evalPosY(const Context& context, const LedContext& ledContext);
		float evalPosZ(const Context& context, const LedContext& ledContext);

		std::shared_ptr<OutputPort<float>> nodeIdOut;
		std::shared_ptr<OutputPort<float>> numLedsOut;
		std::shared_ptr<OutputPort<float>> elapsedOut;
		std::shared_ptr<OutputPort<float>> posXOut;
		std::shared_ptr<OutputPort<float>> posYOut;
		std::shared_ptr<OutputPort<float>> posZOut;
	};

} // namespace Node