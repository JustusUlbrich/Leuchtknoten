#pragma once

#include <map>

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
	int numLeds;

	unsigned long lastUpdate = 0;
	unsigned long delta = 0.f;
	float elapsed = 0.f;

	std::vector<MidiNote> activeNotes;
	// std::unordered_map<uint8_t, bool> midiByNote{};

	// TODO: add more maps for additional channels. Add data for velocity?!
	// TODO: add led positrion mapping
};

struct LedContext
{
	int id;
};
