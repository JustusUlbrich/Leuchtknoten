#include "NodeAnimNumber.hpp"
#include <math.h>

namespace Node
{
	NodeAnimNumber::NodeAnimNumber(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		reset = std::make_shared<InputPort<bool>>("reset", this);
		delay = std::make_shared<InputPort<float>>("delay", this);
		out = std::make_shared<OutputPort<float>>(
			"out",
			this,
			[this](const Context &c, const LedContext &lc) { return eval(c, lc); });
	}

	NodeAnimNumber::~NodeAnimNumber()
	{
	}

	void NodeAnimNumber::update(const float delta, const Context &context, const LedContext &ledContext)
	{
		if (getReset(context, ledContext))
		{
			t = 0.f;
			return;
		}

		if (t >= 1.f)
			return;

		t += delta;
		t = min(t, 1.0f);
	}


	float NodeAnimNumber::getDelay(const Context &context, const LedContext &ledContext)
	{
		float value = 1.f;

		if (delay->connection.has_value())
		{
			auto con = delay->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}

		return value;
	}

	boolean NodeAnimNumber::getReset(const Context &context, const LedContext &ledContext)
	{
		bool value = false;

		if (reset->connection.has_value())
		{
			auto con = reset->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}

		return value;
	}

	float NodeAnimNumber::eval(const Context &context, const LedContext &ledContext)
	{
		return t / getDelay(context, ledContext);
	}

	void NodeAnimNumber::connectOutport(const std::string &portID, Connection<float> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<float>>(out);
	}
} // namespace Node
