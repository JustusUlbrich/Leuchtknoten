#include "NodeAnimNumber.hpp"
#include <math.h>
#include "../../config.hpp"

namespace Node
{
	NodeAnimNumber::NodeAnimNumber(const ArduinoJson::JsonObject& nodeJson, NodeFactory* nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		reset = std::make_shared<InputPort<bool>>("reset", this);
		delay = std::make_shared<InputPort<float>>("delay", this);
		out = std::make_shared<OutputPort<float>>(
			"out",
			this,
			[this](const Context& c, const LedContext& lc) { return eval(c, lc); });

		connectInport(nodeJson, nodeFactory, reset, "reset");
		connectInport(nodeJson, nodeFactory, delay, "delay");

		auto ledCount = nodeFactory->getConfig().leds.count;
		t.resize(ledCount);

		for (int i = 0; i < ledCount; i++)
			t[i] = 0.f;
	}

	NodeAnimNumber::~NodeAnimNumber()
	{
	}

	void NodeAnimNumber::preEval(const Context& context, const LedContext& ledContext)
	{
		// if (getReset(context, ledContext))
		// 	t = 0.f;
		// else
		// t += delta;
	}

	float NodeAnimNumber::getDelay(const Context& context, const LedContext& ledContext)
	{
		float value = 1.f;

		if (delay->connection.has_value())
		{
			auto con = delay->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}

		return value;
	}

	boolean NodeAnimNumber::getReset(const Context& context, const LedContext& ledContext)
	{
		bool value = false;

		if (reset->connection.has_value())
		{
			auto con = reset->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}

		return value;
	}

	float NodeAnimNumber::eval(const Context& context, const LedContext& ledContext)
	{
		if (getReset(context, ledContext))
		{
			t[ledContext.id] = 0.f;
			return 0.f;
		}

		t[ledContext.id] += context.delta;
		return min(t[ledContext.id] / getDelay(context, ledContext), 1.0f);
	}

	void NodeAnimNumber::connectOutport(const std::string& portID, Connection<float>& connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<float>>(out);
	}
} // namespace Node
