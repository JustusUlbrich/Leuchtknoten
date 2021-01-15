#include <Arduino.h>
#include <FastLED.h>
#include "WiFi.h"
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "Spiffs.h"

#include <string>

// Nodes
#include "NodeNetwork/api/INode.hpp"
#include <ArduinoJson.h>
// #include "json.hpp"
#include "NodeNetwork/Generators.hpp"

// Wifi
const char *ssid = "PrettyFlyForAWifi";
const char *password = "***REMOVED***";

// LED Setup
#define LED_PIN 25
#define COLOR_ORDER GRB
#define NUM_LEDS 10

#define BRIGHTNESS 64

enum LedMode
{
	all,
	single,
	pride,
	off
};

uint16_t gDelay = 100;
LedMode gMode = LedMode::all;

std::vector<std::shared_ptr<Node::INode>> gNodes;
bool gNeedUpate = false;

CRGB gColors[NUM_LEDS];
CRGB leds[NUM_LEDS];

// Http Server
AsyncWebServer server(80);

void onUpdateNodes(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
	gNodes.clear();
	Node::fromNetworkJson((char *)data, gNodes);

	Serial.println("Done with network parsing");

	gNeedUpate = true;

	// Serial.println((char *)data);
	request->send(200);
}

void setup()
{
	for (int i = 0; i < NUM_LEDS; i++)
		gColors[i] = CRGB::White;

	delay(2000); // sanity delay
	FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

	FastLED.setBrightness(BRIGHTNESS);

	Serial.begin(9600);

	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.println("Connecting to WiFi..");
	}
	Serial.println("Connected to the WiFi network: " + WiFi.localIP().toString());

	// API
	server.on("/api/mode", HTTP_GET, [](AsyncWebServerRequest *request) {
		gMode = (gMode != LedMode::single) ? LedMode::single : LedMode::pride;
		request->send(200, "text/plain", "Success!");
	});

	server.on("/api/delay", HTTP_GET, [](AsyncWebServerRequest *request) {
		gDelay = (gDelay == 100) ? 10 : 100;
		request->send(200, "text/plain", "Success!");
	});

	server.on(
		"/api/node", HTTP_POST,
		[](AsyncWebServerRequest *request) { request->send(200); },
		[](...) {},
		onUpdateNodes);

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(SPIFFS, "/index.html", String());
	});

	// Web
	SPIFFS.begin();
	server.serveStatic("/", SPIFFS, "/");

	// Options default handler
	server.onNotFound([](AsyncWebServerRequest *request) {
	if (request->method() == HTTP_OPTIONS) {
		request->send(200);
	} else {
		request->send(404);
	} });

	// CORS
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "content-type");
	server.begin();
}

void prideMode()
{
	static uint16_t sPseudotime = 0;
	static uint16_t sLastMillis = 0;
	static uint16_t sHue16 = 0;

	uint8_t sat8 = beatsin88(87, 220, 250);
	uint8_t brightdepth = beatsin88(341, 96, 224);
	uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
	uint8_t msmultiplier = beatsin88(147, 23, 60);

	uint16_t hue16 = sHue16; //gHue * 256;
	uint16_t hueinc16 = beatsin88(113, 1, 3000);

	uint16_t ms = millis();
	uint16_t deltams = ms - sLastMillis;
	sLastMillis = ms;
	sPseudotime += deltams * msmultiplier;
	sHue16 += deltams * beatsin88(400, 5, 9);
	uint16_t brightnesstheta16 = sPseudotime;

	for (uint16_t i = 0; i < NUM_LEDS; i++)
	{
		hue16 += hueinc16;
		uint8_t hue8 = hue16 / 256;

		brightnesstheta16 += brightnessthetainc16;
		uint16_t b16 = sin16(brightnesstheta16) + 32768;

		uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
		uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
		bri8 += (255 - brightdepth);

		CRGB newcolor = CHSV(hue8, sat8, bri8);

		uint16_t pixelnumber = i;
		pixelnumber = (NUM_LEDS - 1) - pixelnumber;

		nblend(leds[pixelnumber], newcolor, 64);
	}
}

void singleMode()
{
	// Move a single white led
	for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1)
	{
		// Turn our current led on to white, then show the leds
		leds[whiteLed] = gColors[whiteLed];

		// Show the leds (only one of which is set to white, from above)
		FastLED.show();

		// Wait a little bit
		delay(gDelay);

		// Turn our current led back to black for the next loop around
		leds[whiteLed] = CRGB::Black;
	}
}

void allMode()
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		// Turn our current led on to white, then show the leds
		leds[i] = gColors[i];
	}
	// Show the leds (only one of which is set to white, from above)
	FastLED.show();
	// Wait a little bit
	delay(gDelay);
}

void loop()
{
	if (gNodes.size() > 0 && gNeedUpate)
	{
		gNeedUpate = false;
		Node::DataRgb rgb;
		for (int i = 0; i < NUM_LEDS; i++)
		{
			LedContext ledCtx;
			ledCtx.id = i;

			gNodes[0]->eval(Context(), ledCtx, rgb);

			// Serial.print("Eval:");
			// Serial.print(rgb.r);
			// Serial.print(rgb.g);
			// Serial.println(rgb.b);

			gColors[i].setRGB(rgb.r, rgb.g, rgb.b);
		}
	}

	switch (gMode)
	{
	case LedMode::all:
		allMode();
		break;
	case LedMode::single:
		singleMode();
		break;
	case LedMode::pride:
		prideMode();
		break;
	default:
		break;
	}
	FastLED.show();
}