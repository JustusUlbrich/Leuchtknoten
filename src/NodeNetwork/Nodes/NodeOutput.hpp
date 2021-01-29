#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"
#include "../api/Connection.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeOutput : public INode
	{

	public:
		explicit NodeOutput(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory);
		~NodeOutput();

		CRGB eval(const Context &context, const LedContext &ledContext);
		std::shared_ptr<InputPort<CRGB>> in;
	};
} // namespace Node