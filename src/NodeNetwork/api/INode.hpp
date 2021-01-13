#pragma once

#include <memory>
#include <vector>

#include "DataRgb.hpp"
#include "../../context.hpp"

// #include "NodeData.hpp"

namespace Node
{
	class INode
	{
	public:
		// TODO: private?
		int id;
		// NodeData data;
		std::vector<int> position;
		std::string name;

		virtual void eval(const Context &context, const LedContext &ledContext, DataRgb &out){};
		virtual void eval(const Context &context, const LedContext &ledContext, int &out){};

		template <typename T>
		std::shared_ptr<T> getPortAs(std::string portID);
	};

} // namespace Node