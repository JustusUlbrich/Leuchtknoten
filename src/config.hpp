#pragma once

#include <string>

struct LedsCfg {
	uint32_t count;
	uint8_t brightness;
};

struct MidiCfg {
	bool enabled;
};

struct SpotifyCfg {
	bool enabled;
	uint32_t updateRateMs;
	std::string market;
	std::string clientId;
	std::string clientSecret;
	std::string refreshToken;
};

struct WifiCfg {
	std::string ssid;
	std::string key;
};

struct Config {
	LedsCfg leds;
	WifiCfg wifi;
	SpotifyCfg spotify;
	MidiCfg midi;
};
