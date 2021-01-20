#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"
#include "../api/Connection.hpp"
#include "../api/DataRgb.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeOutput : public INode
	{

	public:
		explicit NodeOutput(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory);
		~NodeOutput();

		void eval(const Context &context, const LedContext &ledContext, DataRgb &out) override;
		std::shared_ptr<InputPort<DataRgb>> in;
	};
} // namespace Node