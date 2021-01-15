#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeRgb : public INode
	{

	public:
		NodeRgb(/* args */);
		~NodeRgb();

		void eval(const Context &context, const LedContext &ledContext, DataRgb &out) override;
		void connectOutport(const std::string &portID, Connection<DataRgb> &connection) override;

		std::shared_ptr<OutputPort<DataRgb>> out;
		DataRgb value;
	};

	NodeRgb::NodeRgb(/* args */)
	{
		out = std::make_shared<OutputPort<DataRgb>>("num", this);
	}

	NodeRgb::~NodeRgb()
	{
	}

	void NodeRgb::eval(const Context &context, const LedContext &ledContext, DataRgb &out)
	{
		Serial.print("\t\t eval rgb");
		Serial.println(id);

		out = value;
	}

	void NodeRgb::connectOutport(const std::string &portID, Connection<DataRgb> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<DataRgb>>(out);
	}

} // namespace Node