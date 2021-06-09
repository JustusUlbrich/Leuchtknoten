#pragma once

#include "config.hpp"

void convertFromJson(JsonVariantConst src, General& dst) {
	dst.initMode = src["init_mode"] | "";
}

void convertFromJson(JsonVariantConst src, LedInfo& dst) {
	dst.posX = src["pos_x"] | 0.f;
	dst.posY = src["pos_y"] | 0.f;
	dst.posZ = src["pos_z"] | 0.f;
}

NO_INLINE void convertFromJson(JsonVariantConst src, LedsCfg& dst) {
	dst.count = src["count"] | 0;
	dst.brightness = src["brightness"] | 0;

	auto ledInfo = src["led_info"].as<JsonArray>();
	for (auto info : ledInfo)
	{
		auto i = info.as<LedInfo>();
		dst.ledInfo.push_back(i);
	}
}

void convertFromJson(JsonVariantConst src, MidiCfg& dst) {
	dst.enabled = src["enabled"] | true;
}

void convertFromJson(JsonVariantConst src, SpotifyCfg& dst) {
	dst.enabled = src["enabled"] | false;
	dst.updateRateMs = src["update_rate_ms"] | 10000;
	dst.market = src["market"] | "";
	dst.clientId = src["client_id"] | "";
	dst.clientSecret = src["client_secret"] | "";
	dst.refreshToken = src["refresh_token"] | "";
}

void convertFromJson(JsonVariantConst src, WifiCfg& dst) {
	dst.ssid = src["ssid"] | "";
	dst.key = src["key"] | "";
}

void convertFromJson(JsonVariantConst src, Config& dst) {
	dst.general = src["general"].as<General>();
	dst.leds = src["leds"].as<LedsCfg>();
	dst.wifi = src["wifi"].as<WifiCfg>();
	dst.spotify = src["spotify"].as<SpotifyCfg>();
	dst.midi = src["midi"].as<MidiCfg>();
}
