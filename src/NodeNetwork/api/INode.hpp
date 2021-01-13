#pragma once

#include <memory>
#include <vector>

#include "Connection.hpp"
#include "DataRgb.hpp"
#include "../../context.hpp"

// #include "NodeData.hpp"

namespace Node
{
	class INode : std::enable_shared_from_this<INode>
	{
	public:
		// TODO: private?
		int id;
		// NodeData data;
		std::vector<int> position;
		std::string name;

		virtual void eval(const Context &context, const LedContext &ledContext, DataRgb &out){};
		virtual void eval(const Context &context, const LedContext &ledContext, int &out){};

		virtual void connectOutport(const std::string &portID, Connection<DataRgb> &connection) {};
		virtual void connectOutport(const std::string &portID, Connection<int> &connection) {};

		std::shared_ptr<INode> getptr() {
			return shared_from_this();
		}
	};

} // namespace Node