#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeTrigo : public INode
	{

	public:
		explicit NodeTrigo(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory);
		~NodeTrigo();

		void eval(const Context &context, const LedContext &ledContext, const std::string &portId, float &out) override;
		void connectOutport(const std::string &portID, Connection<float> &connection) override;

		std::shared_ptr<InputPort<float>> in;
		std::shared_ptr<OutputPort<float>> out;

		// TODO: enum?
		int mode;
	};
} // namespace Node