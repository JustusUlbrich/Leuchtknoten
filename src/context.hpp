#pragma once

struct Context
{
	int numLeds;
	unsigned long startTime = 0;
	unsigned long lastUpdate = 0;
	float elapsed = 0.f;
	// Add time, elapsed, etc
	// add mapping
};

struct LedContext
{
	int id;
};
