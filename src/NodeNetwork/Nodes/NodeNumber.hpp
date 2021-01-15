#pragma once
#include <string>

#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeNumber : public INode
	{

	public:
		NodeNumber(/* args */);
		~NodeNumber();

		void eval(const Context &context, const LedContext &ledContext, int &out) override;
		void connectOutport(const std::string &portID, Connection<int> &connection) override;

		std::shared_ptr<OutputPort<int>> out;
		int value;
	};

	NodeNumber::NodeNumber(/* args */)
	{
		out = std::make_shared<OutputPort<int>>("num", this);
	}

	NodeNumber::~NodeNumber()
	{
	}

	void NodeNumber::eval(const Context &context, const LedContext &ledContext, int &out)
	{
		out = value;
	}

	void NodeNumber::connectOutport(const std::string &portID, Connection<int> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<int>>(out);
	}

} // namespace Node