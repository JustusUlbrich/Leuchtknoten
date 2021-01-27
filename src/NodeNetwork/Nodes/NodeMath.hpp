#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeMath : public INode
	{

	public:
		explicit NodeMath(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory);
		~NodeMath();

		void connectOutport(const std::string &portID, Connection<float> &connection) override;

		float getIn1(const Context &context, const LedContext &ledContext);
		float getIn2(const Context &context, const LedContext &ledContext);

		float evalAdd(const Context &context, const LedContext &ledContext);
		float evalSub(const Context &context, const LedContext &ledContext);
		float evalMul(const Context &context, const LedContext &ledContext);
		float evalDiv(const Context &context, const LedContext &ledContext);

		std::shared_ptr<InputPort<float>> in1;
		std::shared_ptr<InputPort<float>> in2;
		std::shared_ptr<OutputPort<float>> addOut;
		std::shared_ptr<OutputPort<float>> subOut;
		std::shared_ptr<OutputPort<float>> mulOut;
		std::shared_ptr<OutputPort<float>> divOut;
	};
} // namespace Node