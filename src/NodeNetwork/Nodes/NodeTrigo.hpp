#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

struct Context;

namespace Node
{
	class NodeTrigo : public INode
	{

	public:
		explicit NodeTrigo(const ArduinoJson::JsonObject& nodeJson, NodeFactory* nodeFactory);
		~NodeTrigo();

		void connectOutport(const std::string& portID, Connection<float>& connection) override;

		std::shared_ptr<InputPort<float>> in;
		std::shared_ptr<OutputPort<float>> sinOut;
		std::shared_ptr<OutputPort<float>> cosOut;
		std::shared_ptr<OutputPort<float>> tanOut;

		float getInput(const Context& context, const LedContext& ledContext);
		float evalSin(const Context& context, const LedContext& ledContext);
		float evalCos(const Context& context, const LedContext& ledContext);
		float evalTan(const Context& context, const LedContext& ledContext);
	};
} // namespace Node