#pragma once

#include <string>
#include <vector>

#include "DataRgb.hpp"

namespace Node
{
	struct GradientEntry
	{
		float offset;
		// TODO: maybe use rgb here
		DataRgb color;
	};

	struct DataGradient
	{
		std::vector<GradientEntry> entries;
	};
} // namespace Node
