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
		DataRgb eval(const Context &context, const LedContext &ledContext);
		InputPort<DataRgb> in;
	};

	DataRgb NodeOutput::eval(const Context &context, const LedContext &ledContext)
	{
		if (this->in.connection.has_value())
		{
			auto con = this->in.connection.value();
			return con.fromPort->eval(context, ledContext);
		}

		return DataRgb();
	}

} // namespace Node