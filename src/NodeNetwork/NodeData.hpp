#pragma once

namespace Node
{
	struct DataRgb;
}

namespace Node
{
	using nlohmann::json;

	struct NodeData
	{
		std::shared_ptr<int64_t> num;
		std::shared_ptr<DataRgb> rgb;
	};
} // namespace Node
