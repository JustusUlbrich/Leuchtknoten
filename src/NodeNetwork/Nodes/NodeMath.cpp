#include "NodeMath.hpp"

#include "../../defines.h"

namespace Node
{
	NodeMath::NodeMath(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		in1 = std::make_shared<InputPort<float>>("in1", this);
		in2 = std::make_shared<InputPort<float>>("in2", this);

		addOut = std::make_shared<OutputPort<float>>(
			"add",
			this,
			[this](const Context &c, const LedContext &lc) { return evalAdd(c, lc); });

		subOut = std::make_shared<OutputPort<float>>(
			"sub",
			this,
			[this](const Context &c, const LedContext &lc) { return evalSub(c, lc); });

		mulOut = std::make_shared<OutputPort<float>>(
			"mul",
			this,
			[this](const Context &c, const LedContext &lc) { return evalMul(c, lc); });

		divOut = std::make_shared<OutputPort<float>>(
			"div",
			this,
			[this](const Context &c, const LedContext &lc) { return evalDiv(c, lc); });

		connectInport(nodeJson, nodeFactory, in1, "in1");
		connectInport(nodeJson, nodeFactory, in2, "in2");
	}

	NodeMath::~NodeMath()
	{
	}

	float NodeMath::getIn1(const Context &context, const LedContext &ledContext)
	{
		float value = 0.f;
		if (in1->connection.has_value())
		{
			auto con = in1->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}
		return value;
	}

	float NodeMath::getIn2(const Context &context, const LedContext &ledContext)
	{
		float value = 0.f;

		if (in2->connection.has_value())
		{
			auto con = in2->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}
		return value;
	}

	float NodeMath::evalAdd(const Context &context, const LedContext &ledContext)
	{
		return getIn1(context, ledContext) + getIn2(context, ledContext);
	}

	float NodeMath::evalSub(const Context &context, const LedContext &ledContext)
	{
		return getIn1(context, ledContext) - getIn2(context, ledContext);
	}

	float NodeMath::evalMul(const Context &context, const LedContext &ledContext)
	{
		return getIn1(context, ledContext) * getIn2(context, ledContext);
	}

	float NodeMath::evalDiv(const Context &context, const LedContext &ledContext)
	{
		auto val2 = getIn2(context, ledContext);
		return getIn1(context, ledContext) / (val2 != 0.f ? val2 : 0.0001f);
	}

	void NodeMath::connectOutport(const std::string &portID, Connection<float> &connection)
	{
		if (portID == "add")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(addOut);
		else if (portID == "sub")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(subOut);
		else if (portID == "mul")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(mulOut);
		else if (portID == "div")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(divOut);
	}
} // namespace Node
