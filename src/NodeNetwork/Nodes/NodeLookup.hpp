#pragma once
#include <string>

#include "../api/DataGradient.hpp"
#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeLookup : public INode
	{

	public:
		explicit NodeLookup(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory);
		~NodeLookup();

		void eval(const Context &context, const LedContext &ledContext, const std::string &portId, float &out) override;
		void connectOutport(const std::string &portID, Connection<float> &connection) override;

		std::shared_ptr<OutputPort<float>> nodeIdOut;
		std::shared_ptr<OutputPort<float>> numLedsOut;
		std::shared_ptr<OutputPort<float>> elapsedOut;
	};

} // namespace Node