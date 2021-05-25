#include "NodeMidi.hpp"
#include "../NodeFactory.hpp"
#include <algorithm>
#include <iterator>

namespace Node
{
	NodeMidi::NodeMidi(const ArduinoJson::JsonObject& nodeJson, NodeFactory* nodeFactory)
		: INode(nodeJson, nodeFactory)
	{

		inNoteMin = std::make_shared<InputPort<float>>("noteMin", this);
		inNoteMax = std::make_shared<InputPort<float>>("noteMax", this);

		outNoteOn = std::make_shared<OutputPort<bool>>(
			"noteOn",
			this,
			[this](const Context& c, const LedContext& lc) { return evalNoteOn(c, lc); });

		// outNoteOff = std::make_shared<OutputPort<bool>>(
		// 	"noteOff",
		// 	this,
		// 	[this](const Context &c, const LedContext &lc) { return evalNoteOff(c, lc); });

		outPitch = std::make_shared<OutputPort<float>>(
			"pitch",
			this,
			[this](const Context& c, const LedContext& lc) { return evalPitch(c, lc); });

		outVelocity = std::make_shared<OutputPort<float>>(
			"velo",
			this,
			[this](const Context& c, const LedContext& lc) { return evalVelocity(c, lc); });

		connectInport(nodeJson, nodeFactory, inNoteMin, "noteMin");
		connectInport(nodeJson, nodeFactory, inNoteMax, "noteMax");

		auto ledCount = nodeFactory->getConfig().leds.count;
		lastPitch.resize(ledCount);
		lastVelocity.resize(ledCount);

		for (int i = 0; i < ledCount; i++)
		{
			lastPitch[i] = 0.f;
			lastVelocity[i] = 0.f;
		}
	}

	NodeMidi::~NodeMidi()
	{
	}

	void NodeMidi::postEval(const float delta, const Context& context, const LedContext& ledContext)
	{
	}

	uint8_t NodeMidi::getNoteMin(const Context& context, const LedContext& ledContext)
	{
		uint8_t value = 0;

		if (inNoteMin->connection.has_value())
		{
			auto con = inNoteMin->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}

		return value;
	}

	uint8_t NodeMidi::getNoteMax(const Context& context, const LedContext& ledContext)
	{
		uint8_t value = 0;

		if (inNoteMax->connection.has_value())
		{
			auto con = inNoteMax->connection.value();
			value = con.fromPort->eval(context, ledContext);
		}

		return value;
	}

	bool NodeMidi::evalNoteOn(const Context& context, const LedContext& ledContext)
	{
		// TODO: cache results by copying all results and reuse them. use flag to mark as computed
		const uint8_t min = getNoteMin(context, ledContext);
		const uint8_t max = getNoteMax(context, ledContext);

		const auto noteInRange = [min, max](MidiNote m) { return m.note >= min && m.note <= max; };

		auto inRange =
			std::find_if(context.activeNotes.rbegin(), context.activeNotes.rend(), noteInRange);

		if (inRange == context.activeNotes.rend())
			return false;

		lastPitch[ledContext.id] = (inRange->note - min) / (float)(max - min);
		lastVelocity[ledContext.id] = inRange->velocity;

		return true;
	}

	float NodeMidi::evalPitch(const Context& context, const LedContext& ledContext)
	{
		// TODO overthink this
		return lastPitch[ledContext.id];
	}

	float NodeMidi::evalVelocity(const Context& context, const LedContext& ledContext)
	{
		// TODO overthink this
		return lastVelocity[ledContext.id];
	}

	void NodeMidi::connectOutport(const std::string& portID, Connection<float>& connection)
	{
		if (portID == "pitch")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(outPitch);
		else if (portID == "velo")
			connection.fromPort = std::shared_ptr<OutputPort<float>>(outVelocity);
	}

	void NodeMidi::connectOutport(const std::string& portID, Connection<bool>& connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<bool>>(outNoteOn);
	}
}