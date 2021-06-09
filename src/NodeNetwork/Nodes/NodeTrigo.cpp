#include "NodeTrigo.hpp"
#include <math.h>
#include "../../context.hpp"

namespace Node
{
	NodeTrigo::NodeTrigo(const ArduinoJson::JsonObject& nodeJson, NodeFactory* nodeFactory)
		: INode(nodeJson, nodeFactory)
	{
		in = std::make_shared<InputPort<float>>("in", this);
		sinOut = std::make_shared<OutputPort<float>>(
			"sin",
			this,
			[this](const Context& c, const LedContext& lc) { return evalSin(c, lc); });
		cosOut = std::make_shared<OutputPort<float>>(
			"cos",
			this,
			[this](const Context& c, const LedContext& lc) { return evalCos(c, lc); });
		tanOut = std::make_shared<OutputPort<float>>(
			"tan",
			this,
			[this](const Context& c, const LedContext& lc) { return evalTan(c, lc); });

		connectInport(nodeJson, nodeFactory, in, "in");
	}

	NodeTrigo::~NodeTrigo()
	{
	}

	float NodeTrigo::getInput(const Context& context, const LedContext& ledContext)
	{
		float value = 0.f;

		if (in->connection.has_value())
		{
			auto con = in->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}

		return value;
	}

	float NodeTrigo::evalSin(const Context& context, const LedContext& ledContext)
	{
		return sinf(getInput(context, ledContext));
	}

	float NodeTrigo::evalCos(const Context& context, const LedContext& ledContext)
	{
		return cosf(getInput(context, ledContext));
	}

	float NodeTrigo::evalTan(const Context& context, const LedContext& ledContext)
	{
		return tanf(getInput(context, ledContext));
	}

	void NodeTrigo::connectOutport(const std::string& portID, Connection<float>& connection)
	{
		if (portID == "sin")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(sinOut);
		else if (portID == "cos")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(cosOut);
		else if (portID == "tan")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(tanOut);
	}
} // namespace Node
