#include "NodeGradient.hpp"
#include "../NodeFactory.hpp"

namespace Node
{
	NodeGradient::NodeGradient(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		scaleIn = std::make_shared<InputPort<float>>("scale", this);
		out = std::make_shared<OutputPort<CRGB>>(
			"num",
			this,
			[this](const Context &c, const LedContext &lc) { return evalRgb(c, lc); });

		auto jsonGradients = nodeJson["data"]["gradient"].as<JsonArray>();
		for (JsonVariant v : jsonGradients)
		{
			Node::GradientEntry e;
			e.offset = v["offset"].as<float>();

			uint32_t data = (uint32_t)strtol(v["color"].as<std::string>().substr(1).c_str(), NULL, 16);

			e.color = CRGB{data};

			gradient.entries.push_back(e);
		}

		connectInport(nodeJson, nodeFactory, scaleIn, "scale");
	}

	NodeGradient::~NodeGradient()
	{
	}

	CRGB NodeGradient::evalRgb(const Context &context, const LedContext &ledContext)
	{
		// float scale = 0.5;
		// DEBUG
		float scale = ledContext.id / (float)context.numLeds;
		// END DEBUG

		if (scaleIn->connection.has_value())
		{
			auto con = scaleIn->connection.value();
			scale = con.fromPort->eval(context, ledContext);
		}

		if (gradient.entries.size() == 0)
		{
			return CRGB::Black;
		}

		if (gradient.entries.size() == 1 || scale < gradient.entries[0].offset)
		{
			return gradient.entries[0].color;
		}

		for (size_t i = 0; i < gradient.entries.size() - 1; i++)
		{
			if (scale > gradient.entries[i + 1].offset)
				continue;

			const auto cLeft = gradient.entries[i];
			const auto cRight = gradient.entries[i + 1];

			// TODO: clamp and verify
			const auto sOff = (scale - cLeft.offset) / (cRight.offset - cLeft.offset);

			CRGB out{};
			out.r = ((1.0 - sOff) * cLeft.color.r + sOff * cRight.color.r);
			out.g = ((1.0 - sOff) * cLeft.color.g + sOff * cRight.color.g);
			out.b = ((1.0 - sOff) * cLeft.color.b + sOff * cRight.color.b);
			return out;
		}

		return gradient.entries[gradient.entries.size() - 1].color;
	}

	void NodeGradient::updateValue(const ArduinoJson::JsonObject &nodeJson)
	{
		auto jsonGradients = nodeJson["data"]["gradient"].as<JsonArray>();
		for (JsonVariant v : jsonGradients)
		{
			Node::GradientEntry e;
			e.offset = v["offset"].as<float>();

			uint32_t data = (uint32_t)strtol(v["color"].as<std::string>().substr(1).c_str(), NULL, 16);

			e.color = CRGB{data};

			gradient.entries.push_back(e);
		}
	}


	void NodeGradient::connectOutport(const std::string &portID, Connection<CRGB> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<CRGB>>(out);
	}

} // namespace Node