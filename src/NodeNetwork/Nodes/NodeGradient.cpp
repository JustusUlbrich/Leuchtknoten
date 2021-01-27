#include "NodeGradient.hpp"
#include "../NodeFactory.hpp"

namespace Node
{
	NodeGradient::NodeGradient(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		scaleIn = std::make_shared<InputPort<float>>("scale", this);
		out = std::make_shared<OutputPort<DataRgb>>(
			"num",
			this,
			[this](const Context &c, const LedContext &lc) { return evalRgb(c, lc); });

		auto jsonGradients = nodeJson["data"]["gradient"].as<JsonArray>();
		for (JsonVariant v : jsonGradients)
		{
			Node::GradientEntry e;
			e.offset = v["offset"].as<float>();
			e.color = DataRgb::fromHex(v["color"].as<std::string>());

			gradient.entries.push_back(e);
		}

		connectInport(nodeJson, nodeFactory, scaleIn, "scale");
	}

	NodeGradient::~NodeGradient()
	{
	}

	DataRgb NodeGradient::evalRgb(const Context &context, const LedContext &ledContext)
	{
		// float scale = 0.5;
		// DEBUG
		float scale = ledContext.id / 10.0f;
		// END DEBUG

		if (scaleIn->connection.has_value())
		{
			auto con = scaleIn->connection.value();
			scale = con.fromPort->eval(context, ledContext);
		}

		if (gradient.entries.size() == 0)
		{
			return DataRgb();
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

			DataRgb out{};
			out.r = ((1.0 - sOff) * cLeft.color.r + sOff * cRight.color.r);
			out.g = ((1.0 - sOff) * cLeft.color.g + sOff * cRight.color.g);
			out.b = ((1.0 - sOff) * cLeft.color.b + sOff * cRight.color.b);
			return out;
		}

		return gradient.entries[gradient.entries.size() - 1].color;
	}

	void NodeGradient::connectOutport(const std::string &portID, Connection<DataRgb> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<DataRgb>>(out);
	}

} // namespace Node