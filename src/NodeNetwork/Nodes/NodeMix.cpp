#include "NodeMix.hpp"
#include "../NodeFactory.hpp"

namespace Node
{
	NodeMix::NodeMix(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		c1In = std::make_shared<InputPort<CRGB>>("c1", this);
		c2In = std::make_shared<InputPort<CRGB>>("c2", this);
		scaleIn = std::make_shared<InputPort<float>>("scale", this);

		mixOut = std::make_shared<OutputPort<CRGB>>(
			"mix",
			this,
			[this](const Context &c, const LedContext &lc) { return evalRgb(c, lc); });

		connectInport(nodeJson, nodeFactory, c1In, "c1");
		connectInport(nodeJson, nodeFactory, c2In, "c2");
		connectInport(nodeJson, nodeFactory, scaleIn, "scale");
	}

	NodeMix::~NodeMix()
	{
	}

	CRGB NodeMix::evalRgb(const Context &context, const LedContext &ledContext)
	{
		float scale = ledContext.id / (float)context.numLeds;
		CRGB c1{}, c2{};

		if (scaleIn->connection.has_value())
		{
			auto con = scaleIn->connection.value();
			scale = con.fromPort->eval(context, ledContext);
		}
		if (c1In->connection.has_value())
		{
			auto con = c1In->connection.value();
			c1 = con.fromPort->eval(context, ledContext);
		}
		if (c2In->connection.has_value())
		{
			auto con = c2In->connection.value();
			c2 = con.fromPort->eval(context, ledContext);
		}

		if (scale <= 0.f)
			return c1;

		if (scale >= 1.f)
			return c1;

		c1.r = scale * c1.r + (1.f - scale) * c2.r;
		c1.g = scale * c1.g + (1.f - scale) * c2.g;
		c1.b = scale * c1.b + (1.f - scale) * c2.b;

		return c1;
	}

	void NodeMix::connectOutport(const std::string &portID, Connection<CRGB> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<CRGB>>(mixOut);
	}

} // namespace Node