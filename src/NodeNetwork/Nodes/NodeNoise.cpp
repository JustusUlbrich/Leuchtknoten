#include "NodeNoise.hpp"
#include <FastLED.h>

#include "../../defines.h"

namespace Node
{
	NodeNoise::NodeNoise(const ArduinoJson::JsonObject& nodeJson, NodeFactory* nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		inX = std::make_shared<InputPort<float>>("inX", this);
		inY = std::make_shared<InputPort<float>>("inY", this);
		inZ = std::make_shared<InputPort<float>>("inZ", this);

		dim1Out = std::make_shared<OutputPort<float>>(
			"dim1",
			this,
			[this](const Context& c, const LedContext& lc) { return evalDim1(c, lc); });

		dim2Out = std::make_shared<OutputPort<float>>(
			"dim2",
			this,
			[this](const Context& c, const LedContext& lc) { return evalDim2(c, lc); });

		dim3Out = std::make_shared<OutputPort<float>>(
			"dim3",
			this,
			[this](const Context& c, const LedContext& lc) { return evalDim3(c, lc); });

		connectInport(nodeJson, nodeFactory, inX, "inX");
		connectInport(nodeJson, nodeFactory, inY, "inY");
		connectInport(nodeJson, nodeFactory, inZ, "inZ");
	}

	NodeNoise::~NodeNoise()
	{
	}

	float NodeNoise::getInX(const Context& context, const LedContext& ledContext)
	{
		float value = 0.f;
		if (inX->connection.has_value())
		{
			auto con = inX->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}
		return value;
	}

	float NodeNoise::getInY(const Context& context, const LedContext& ledContext)
	{
		float value = 0.f;
		if (inY->connection.has_value())
		{
			auto con = inY->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}
		return value;
	}

	float NodeNoise::getInZ(const Context& context, const LedContext& ledContext)
	{
		float value = 0.f;
		if (inZ->connection.has_value())
		{
			auto con = inZ->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}
		return value;
	}

	float NodeNoise::evalDim1(const Context& context, const LedContext& ledContext)
	{
		return inoise16(
			getInX(context, ledContext)
		) / 65535.f;
	}

	float NodeNoise::evalDim2(const Context& context, const LedContext& ledContext)
	{
		return inoise16(
			getInX(context, ledContext),
			getInY(context, ledContext)
		) / 65535.f;
	}

	float NodeNoise::evalDim3(const Context& context, const LedContext& ledContext)
	{
		return inoise16(
			getInX(context, ledContext),
			getInY(context, ledContext),
			getInZ(context, ledContext)
		) / 65535.f;
	}

	void NodeNoise::connectOutport(const std::string& portID, Connection<float>& connection)
	{
		if (portID == "dim1")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(dim1Out);
		else if (portID == "dim2")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(dim2Out);
		else if (portID == "dim3")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(dim3Out);
	}
} // namespace Node
