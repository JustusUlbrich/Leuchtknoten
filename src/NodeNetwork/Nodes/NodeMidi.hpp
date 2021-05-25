#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"
#include "../../defines.h"

namespace Node
{
	class NodeMidi : public INode
	{

	public:
		explicit NodeMidi(const ArduinoJson::JsonObject& nodeJson, NodeFactory* nodeFactory);
		~NodeMidi();

		void postEval(const float delta, const Context& context, const LedContext& ledContext);

		bool evalNoteOn(const Context& context, const LedContext& ledContext);
		bool evalNoteOff(const Context& context, const LedContext& ledContext);
		float evalPitch(const Context& context, const LedContext& ledContext);
		float evalVelocity(const Context& context, const LedContext& ledContext);

		uint8_t getNoteMin(const Context& context, const LedContext& ledContext);
		uint8_t getNoteMax(const Context& context, const LedContext& ledContext);

		void connectOutport(const std::string& portID, Connection<float>& connection) override;
		void connectOutport(const std::string& portID, Connection<bool>& connection) override;

		// TODO overthink this
		std::vector<float> lastPitch;
		std::vector<float> lastVelocity;

		std::shared_ptr<InputPort<float>> inNoteMin;
		std::shared_ptr<InputPort<float>> inNoteMax;
		std::shared_ptr<OutputPort<bool>> outNoteOn;
		// TODO: needed?
		// std::shared_ptr<OutputPort<bool>> outNoteOff;
		std::shared_ptr<OutputPort<float>> outPitch;
		std::shared_ptr<OutputPort<float>> outVelocity;
	};

} // namespace Node