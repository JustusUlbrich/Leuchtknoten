#pragma once

#include <WString.h>
#include <vector>
#include <AsyncJson.h>

struct General {
	String initMode;
};


struct LedInfo {
	float posX;
	float posY;
	float posZ;
};

struct LedsCfg {
	uint32_t count;
	uint8_t brightness;
	std::vector<LedInfo> ledInfo;
};

struct MidiCfg {
	bool enabled;
};

struct SpotifyCfg {
	bool enabled;
	uint32_t updateRateMs;
	String market;
	String clientId;
	String clientSecret;
	String refreshToken;
};

struct WifiCfg {
	String ssid;
	String key;
};

struct Config {
	General general;
	LedsCfg leds;
	WifiCfg wifi;
	SpotifyCfg spotify;
	MidiCfg midi;
};