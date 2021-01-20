#pragma once

#include <Arduino.h>

#include <string>

namespace Node
{
	struct DataRgb
	{
		int r = 0;
		int g = 0;
		int b = 0;

		static DataRgb fromHex(std::string color)
		{
			// TODO: error check
			DataRgb out;

			uint32_t data = (uint32_t)strtol(color.substr(1).c_str(), NULL, 16);
			out.r = ((data >> 16) & 0xFF);
			out.g = ((data >> 8) & 0xFF);
			out.b = ((data >> 0) & 0xFF);

			return out;
		}

		// DataRgb operator+(const DataRgb &other)
		// {
		// 	DataRgb out;
		// 	out.r = this->r + other.r;
		// 	out.g = this->g + other.g;
		// 	out.b = this->b + other.b;
		// 	return out;
		// }
	};
} // namespace Node
