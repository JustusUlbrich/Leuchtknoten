#pragma once

#include "defines.h"
#include <map>
#include "config.hpp"

struct CRGBW;

struct MidiNote
{
	uint8_t channel;
	uint8_t note;
	uint8_t velocity;

	MidiNote(uint8_t _channel, uint8_t _note, uint8_t _velocity)
		: channel(_channel), note(_note), velocity(_velocity)
	{
	}
};

struct Context
{
	unsigned long lastUpdate = 0;
	unsigned long delta = 0.f;
	float elapsed = 0.f;

	std::vector<MidiNote> activeNotes;
	// std::unordered_map<uint8_t, bool> midiByNote{};

	// TODO: add more maps for additional channels. Add data for velocity?!
	// TODO: add led positrion mapping

	unsigned long lastSpotifyUpdate = 0;
	long spotifyDuration = 0;
	long spotifyProgress = 0;

	// TODO: Think about moving this out of "context"?
	Config config;

	// Internal
	std::vector<CRGBW> colorBuffer;
#if LED_USE_RGBW
	std::vector<CRGBW> ledBuffer;
#else
	std::vector<CRGB> ledBuffer;
#endif
};

struct LedContext
{
	int id;
};
