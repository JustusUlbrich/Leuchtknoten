#pragma once

#include <map>

// struct MidiNote
// {
// 	byte channel;
// 	byte note;
// 	byte velocity;
// };

struct Context
{
	int numLeds;
	unsigned long startTime = 0;
	unsigned long lastUpdate = 0;
	float elapsed = 0.f;

	std::unordered_map<byte, bool> midiByNote{};

	// TODO: add more maps for additional channels. Add data for velocity?!
	// TODO: add led positrion mapping
};

struct LedContext
{
	int id;
};
