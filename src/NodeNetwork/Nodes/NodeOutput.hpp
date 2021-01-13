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
		in = std::make_shared<InputPort<DataRgb>>("rgb", getptr());
	}

	NodeOutput::~NodeOutput()
	{

	}

	void NodeOutput::eval(const Context &context, const LedContext &ledContext, DataRgb &out)
	{
		if (this->in->connection.has_value())
		{
			auto con = this->in->connection.value();
			out = con.fromPort->eval(context, ledContext);
		}
	}

} // namespace Node