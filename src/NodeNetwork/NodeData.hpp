#pragma once
#include <memory>
namespace Node
{
	struct DataRgb;
}

namespace Node
{
	struct NodeData
	{
		std::shared_ptr<int> num;
		std::shared_ptr<DataRgb> rgb;
	};
} // namespace Node
