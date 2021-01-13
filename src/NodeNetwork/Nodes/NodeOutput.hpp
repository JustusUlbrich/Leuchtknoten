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
		void eval(const Context &context, const LedContext &ledContext, DataRgb &out) override;
		InputPort<DataRgb> in;
	};

	void NodeOutput::eval(const Context &context, const LedContext &ledContext, DataRgb &out)
	{
		if (this->in.connection.has_value())
		{
			auto con = this->in.connection.value();
			out = con.fromPort->eval(context, ledContext);
		}
	}

} // namespace Node