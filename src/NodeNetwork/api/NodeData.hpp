#pragma once

#include "DataRgb.hpp"

#include "variant.hpp"
using namespace nonstd;

namespace Node
{
	struct NodeData
	{
		nonstd::variant<int, DataRgb> value;
	};
} // namespace Node
