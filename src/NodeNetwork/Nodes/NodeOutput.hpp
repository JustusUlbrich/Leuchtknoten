#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"
#include "../api/Connection.hpp"
#include "../api/DataRgb.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeOutput : public INode
	{

	public:
		NodeOutput(/* args */);
		~NodeOutput();

		void eval(const Context &context, const LedContext &ledContext, DataRgb &out) override;
		std::shared_ptr<InputPort<DataRgb>> in;
	};

	NodeOutput::NodeOutput()
	{
		in = std::make_shared<InputPort<DataRgb>>("rgb", this);
	}

	NodeOutput::~NodeOutput()
	{
	}

	void NodeOutput::eval(const Context &context, const LedContext &ledContext, DataRgb &out)
	{
		Serial.print("\t\t eval at node: ");
		Serial.println(name.c_str());

		if (in->connection)
		{
			Serial.print("\t\t\t eval connection");

			auto con = *in->connection;
			if (con.fromPort != nullptr)
				out = con.fromPort->eval(context, ledContext);
			else
				Serial.print("\t\t\t fromPort empty :(");
		}
		else
		{
			Serial.print("\t\t\t connection empty :(");
		}

		Serial.println("\t\t done eval output");
	}

} // namespace Node