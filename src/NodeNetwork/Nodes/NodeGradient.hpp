#pragma once
#include <string>

#include "../api/DataGradient.hpp"
#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeGradient : public INode
	{

	public:
		explicit NodeGradient(const ArduinoJson::JsonObject &nodeJson, NodeFactory *nodeFactory);
		~NodeGradient();

		void eval(const Context &context, const LedContext &ledContext, DataRgb &out) override;
		void connectOutport(const std::string &portID, Connection<DataRgb> &connection) override;

		std::shared_ptr<InputPort<float>> scaleIn;
		std::shared_ptr<OutputPort<DataRgb>> out;
		DataGradient gradient;
	};

} // namespace Node