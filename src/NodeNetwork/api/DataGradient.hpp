#pragma once
#include <FastLED.h>
#include <string>
#include <vector>

namespace Node
{
	struct GradientEntry
	{
		float offset;
		CRGB color;
	};

	struct DataGradient
	{
		std::vector<GradientEntry> entries;
	};
} // namespace Node
