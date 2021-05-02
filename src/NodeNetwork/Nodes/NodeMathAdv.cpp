#include "NodeMathAdv.hpp"

#include "../../defines.h"

namespace Node
{
	NodeMathAdv::NodeMathAdv(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		in1 = std::make_shared<InputPort<float>>("in1", this);
		in2 = std::make_shared<InputPort<float>>("in2", this);

		stepOut = std::make_shared<OutputPort<float>>(
			"step",
			this,
			[this](const Context &c, const LedContext &lc) { return evalStep(c, lc); });

		modOut = std::make_shared<OutputPort<float>>(
			"mod",
			this,
			[this](const Context &c, const LedContext &lc) { return evalMod(c, lc); });

		powOut = std::make_shared<OutputPort<float>>(
			"pow",
			this,
			[this](const Context &c, const LedContext &lc) { return evalPow(c, lc); });

		absOut = std::make_shared<OutputPort<float>>(
			"abs",
			this,
			[this](const Context &c, const LedContext &lc) { return evalAbs(c, lc); });

		connectInport(nodeJson, nodeFactory, in1, "in1");
		connectInport(nodeJson, nodeFactory, in2, "in2");
	}

	NodeMathAdv::~NodeMathAdv()
	{
	}

	float NodeMathAdv::getIn1(const Context &context, const LedContext &ledContext)
	{
		float value = 0.f;
		if (in1->connection.has_value())
		{
			auto con = in1->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}
		return value;
	}

	float NodeMathAdv::getIn2(const Context &context, const LedContext &ledContext)
	{
		float value = 0.f;

		if (in2->connection.has_value())
		{
			auto con = in2->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}
		return value;
	}

	float NodeMathAdv::evalStep(const Context &context, const LedContext &ledContext)
	{
		float step = getIn2(context, ledContext);
		return getIn1(context, ledContext) > step ? 1.f : 0.f;
	}

	float NodeMathAdv::evalMod(const Context &context, const LedContext &ledContext)
	{
		return fmod(getIn1(context, ledContext), getIn2(context, ledContext));
	}

	float NodeMathAdv::evalPow(const Context &context, const LedContext &ledContext)
	{
		return powf(getIn1(context, ledContext), getIn2(context, ledContext));
	}

	float NodeMathAdv::evalAbs(const Context &context, const LedContext &ledContext)
	{
		return fabs(getIn1(context, ledContext));
	}

	void NodeMathAdv::connectOutport(const std::string &portID, Connection<float> &connection)
	{
		if (portID == "step")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(stepOut);
		else if (portID == "mod")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(modOut);
		else if (portID == "pow")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(powOut);
		else if (portID == "abs")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(absOut);
	}
} // namespace Node
