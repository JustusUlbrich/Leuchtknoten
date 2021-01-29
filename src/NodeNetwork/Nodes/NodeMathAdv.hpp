#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeMathAdv : public INode
	{

	public:
		explicit NodeMathAdv(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory);
		~NodeMathAdv();

		void connectOutport(const std::string &portID, Connection<float> &connection) override;

		float getIn1(const Context &context, const LedContext &ledContext);
		float getIn2(const Context &context, const LedContext &ledContext);

		float evalStep(const Context &context, const LedContext &ledContext);
		float evalMod(const Context &context, const LedContext &ledContext);

		std::shared_ptr<InputPort<float>> in1;
		std::shared_ptr<InputPort<float>> in2;
		std::shared_ptr<OutputPort<float>> stepOut;
		std::shared_ptr<OutputPort<float>> modOut;
	};
} // namespace Node