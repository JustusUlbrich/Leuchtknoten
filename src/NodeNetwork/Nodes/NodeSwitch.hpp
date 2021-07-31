#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeSwitch : public INode
	{

	public:
		explicit NodeSwitch(const ArduinoJson::JsonObject& nodeJson, NodeFactory* nodeFactory);
		~NodeSwitch();

		CRGB evalRgb(const Context& context, const LedContext& ledContext);

		void connectOutport(const std::string& portID, Connection<CRGB>& connection) override;

		std::shared_ptr<InputPort<CRGB>> c0;
		std::shared_ptr<InputPort<CRGB>> c1;
		std::shared_ptr<InputPort<CRGB>> c2;
		std::shared_ptr<InputPort<CRGB>> c3;
		std::shared_ptr<InputPort<CRGB>> c4;
		std::shared_ptr<InputPort<CRGB>> c5;
		std::shared_ptr<InputPort<CRGB>> c6;
		std::shared_ptr<InputPort<CRGB>> c7;
		std::shared_ptr<InputPort<CRGB>> c8;
		std::shared_ptr<InputPort<CRGB>> c9;
		std::shared_ptr<InputPort<CRGB>> c10;
		std::shared_ptr<InputPort<CRGB>> c11;
		std::shared_ptr<InputPort<CRGB>> c12;

		std::shared_ptr<InputPort<float>> scale;

		std::shared_ptr<OutputPort<CRGB>> out;
		
	private:
		CRGB safeEvalPort(const Context& context, const LedContext& ledContext, std::shared_ptr<InputPort<CRGB>>& port);
	};

} // namespace Node