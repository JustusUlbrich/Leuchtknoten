#include "NodeSetHSV.hpp"
#include "../NodeFactory.hpp"
#include <hsv2rgb.h>

namespace Node
{
	NodeSetHSV::NodeSetHSV(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		colorIn = std::make_shared<InputPort<CRGB>>("color", this);
		hIn = std::make_shared<InputPort<float>>("h", this);
		sIn = std::make_shared<InputPort<float>>("s", this);
		vIn = std::make_shared<InputPort<float>>("v", this);

		rgbOut = std::make_shared<OutputPort<CRGB>>(
			"rgb",
			this,
			[this](const Context &c, const LedContext &lc) { return evalRgb(c, lc); });

		connectInport(nodeJson, nodeFactory, colorIn, "color");
		connectInport(nodeJson, nodeFactory, hIn, "h");
		connectInport(nodeJson, nodeFactory, sIn, "s");
		connectInport(nodeJson, nodeFactory, vIn, "v");
	}

	NodeSetHSV::~NodeSetHSV()
	{
	}

	CRGB NodeSetHSV::evalRgb(const Context &context, const LedContext &ledContext)
	{
		float h = -1.f, s = -1.f, v = -1.f;
		CRGB color{};

		if (colorIn->connection.has_value())
		{
			auto con = colorIn->connection.value();
			color = con.fromPort->eval(context, ledContext);
		}

		if (hIn->connection.has_value())
		{
			auto con = hIn->connection.value();
			h = con.fromPort->eval(context, ledContext);
		}
		if (sIn->connection.has_value())
		{
			auto con = sIn->connection.value();
			s = con.fromPort->eval(context, ledContext);
		}
		if (vIn->connection.has_value())
		{
			auto con = vIn->connection.value();
			v = con.fromPort->eval(context, ledContext);
		}

		if (h < 0.f && s < 0.f && v < 0.f)
			return color;

		CHSV hsv = rgb2hsv_approximate(color);
		hsv.h = h >= 0.f ? 255 * h : hsv.h;
		hsv.s = s >= 0.f ? 255 * s : hsv.s;
		hsv.v = v >= 0.f ? 255 * v : hsv.v;

		hsv2rgb_rainbow(hsv, color);

		return color;
	}

	void NodeSetHSV::connectOutport(const std::string &portID, Connection<CRGB> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<CRGB>>(rgbOut);
	}

} // namespace Node