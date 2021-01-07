#pragma once

#include "variant.hpp"
using namespace nonstd;

namespace Node
{
	struct DataRgb;
}

namespace Node
{
	struct NodeData
	{
		nonstd::variant<int, DataRgb> data;

		// std::shared_ptr<int> num;
		// std::shared_ptr<DataRgb> rgb;
	};
} // namespace Node
