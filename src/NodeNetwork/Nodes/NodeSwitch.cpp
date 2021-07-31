#include "NodeSwitch.hpp"
#include "../NodeFactory.hpp"

namespace Node
{
	NodeSwitch::NodeSwitch(const ArduinoJson::JsonObject& nodeJson, NodeFactory* nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		// TODO: make dynamic (array etc)

		c0 = std::make_shared<InputPort<CRGB>>("c0", this);
		c1 = std::make_shared<InputPort<CRGB>>("c1", this);
		c2 = std::make_shared<InputPort<CRGB>>("c2", this);
		c3 = std::make_shared<InputPort<CRGB>>("c3", this);
		c4 = std::make_shared<InputPort<CRGB>>("c4", this);
		c5 = std::make_shared<InputPort<CRGB>>("c5", this);
		c6 = std::make_shared<InputPort<CRGB>>("c6", this);
		c7 = std::make_shared<InputPort<CRGB>>("c7", this);
		c8 = std::make_shared<InputPort<CRGB>>("c8", this);
		c9 = std::make_shared<InputPort<CRGB>>("c9", this);
		c10 = std::make_shared<InputPort<CRGB>>("c10", this);
		c11 = std::make_shared<InputPort<CRGB>>("c11", this);
		c12 = std::make_shared<InputPort<CRGB>>("c12", this);

		scale = std::make_shared<InputPort<float>>("scale", this);
		out = std::make_shared<OutputPort<CRGB>>(
			"rgb",
			this,
			[this](const Context& c, const LedContext& lc) { return evalRgb(c, lc); });

		connectInport(nodeJson, nodeFactory, c0, "c0");
		connectInport(nodeJson, nodeFactory, c1, "c1");
		connectInport(nodeJson, nodeFactory, c2, "c2");
		connectInport(nodeJson, nodeFactory, c3, "c3");
		connectInport(nodeJson, nodeFactory, c4, "c4");
		connectInport(nodeJson, nodeFactory, c5, "c5");
		connectInport(nodeJson, nodeFactory, c6, "c6");
		connectInport(nodeJson, nodeFactory, c7, "c7");
		connectInport(nodeJson, nodeFactory, c8, "c8");
		connectInport(nodeJson, nodeFactory, c9, "c9");
		connectInport(nodeJson, nodeFactory, c10, "c10");
		connectInport(nodeJson, nodeFactory, c11, "c11");
		connectInport(nodeJson, nodeFactory, c12, "c12");

		connectInport(nodeJson, nodeFactory, scale, "scale");
	}

	NodeSwitch::~NodeSwitch()
	{
	}

	CRGB NodeSwitch::evalRgb(const Context& context, const LedContext& ledContext)
	{
		int switchScale = ledContext.id;

		if (scale->connection.has_value())
		{
			auto con = scale->connection.value();
			switchScale = (int)con.fromPort->eval(context, ledContext);
		}

		// TODO: make size dynamic
		switchScale = switchScale % 12;

		CRGB color = CRGB::Black;

		switch (switchScale)
		{
		case 0:
			color = safeEvalPort(context, ledContext, c0);
			break;
		case 1:
			color = safeEvalPort(context, ledContext, c1);
			break;
		case 2:
			color = safeEvalPort(context, ledContext, c2);
			break;
		case 3:
			color = safeEvalPort(context, ledContext, c3);
			break;
		case 4:
			color = safeEvalPort(context, ledContext, c4);
			break;
		case 5:
			color = safeEvalPort(context, ledContext, c5);
			break;
		case 6:
			color = safeEvalPort(context, ledContext, c6);
			break;
		case 7:
			color = safeEvalPort(context, ledContext, c7);
			break;
		case 8:
			color = safeEvalPort(context, ledContext, c8);
			break;
		case 9:
			color = safeEvalPort(context, ledContext, c9);
			break;
		case 10:
			color = safeEvalPort(context, ledContext, c10);
			break;
		case 11:
			color = safeEvalPort(context, ledContext, c11);
			break;
		case 12:
			color = safeEvalPort(context, ledContext, c12);
			break;
		default:
			break;
		}

		return color;
	}


	CRGB NodeSwitch::safeEvalPort(
		const Context& context,
		const LedContext& ledContext,
		std::shared_ptr<InputPort<CRGB>>& port)
	{
		CRGB color = CRGB::Black;

		if (port->connection.has_value())
		{
			auto con = port->connection.value();
			color = con.fromPort->eval(context, ledContext);
		}

		return color;
	}

	void NodeSwitch::connectOutport(const std::string& portID, Connection<CRGB>& connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<CRGB>>(out);
	}

} // namespace Node