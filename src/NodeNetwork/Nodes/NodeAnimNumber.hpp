#pragma once
#include "../../defines.h"
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeAnimNumber : public INode
	{

	public:
		explicit NodeAnimNumber(const ArduinoJson::JsonObject& nodeJson, NodeFactory* nodeFactory);
		~NodeAnimNumber();

		void connectOutport(const std::string& portID, Connection<float>& connection) override;

		std::shared_ptr<InputPort<bool>> reset;
		std::shared_ptr<InputPort<float>> delay;
		std::shared_ptr<OutputPort<float>> out;

		// TODO: name?
		std::vector<float> t;

		void preEval(const Context& context, const LedContext& ledContext) override;

		float getDelay(const Context& context, const LedContext& ledContext);
		boolean getReset(const Context& context, const LedContext& ledContext);
		float eval(const Context& context, const LedContext& ledContext);
	};
} // namespace Node