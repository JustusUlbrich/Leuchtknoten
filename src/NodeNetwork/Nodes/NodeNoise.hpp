#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeNoise : public INode
	{

	public:
		explicit NodeNoise(const ArduinoJson::JsonObject& nodeJson, NodeFactory* nodeFactory);
		~NodeNoise();

		void connectOutport(const std::string& portID, Connection<float>& connection) override;

		float getInX(const Context& context, const LedContext& ledContext);
		float getInY(const Context& context, const LedContext& ledContext);
		float getInZ(const Context& context, const LedContext& ledContext);

		float evalDim1(const Context& context, const LedContext& ledContext);
		float evalDim2(const Context& context, const LedContext& ledContext);
		float evalDim3(const Context& context, const LedContext& ledContext);

		std::shared_ptr<InputPort<float>> inX;
		std::shared_ptr<InputPort<float>> inY;
		std::shared_ptr<InputPort<float>> inZ;
		std::shared_ptr<OutputPort<float>> dim1Out;
		std::shared_ptr<OutputPort<float>> dim2Out;
		std::shared_ptr<OutputPort<float>> dim3Out;
	};
} // namespace Node