#include <Arduino.h>
#include "freertos/FreeRTOS.h"

#include <FastLED.h>
#include "WiFi.h"
#include <AsyncTCP.h>
#include <AsyncJson.h>

#include "ESPAsyncWebServer.h"
#include "Spiffs.h"

#include <unordered_map>
#include <string>

#include "defines.h"

// Nodes
#include "NodeNetwork/api/INode.hpp"
#include <ArduinoJson.h>
// #include "json.hpp"
#include "NodeNetwork/Generators.hpp"

// Wifi
const char *ssid = "PrettyFlyForAWifi";
const char *password = "***REMOVED***";

enum LedMode
{
	all,
	single,
	pride,
	off
};

SemaphoreHandle_t gNetworkSemaphore;

uint16_t gDelay = 100;
LedMode gMode = LedMode::all;

std::string gRootId = "";
std::unordered_map<std::string, std::shared_ptr<Node::INode>> gNodes;
bool gNeedUpate = false;

Context gContext;

CRGB gColors[NUM_LEDS];
CRGB leds[NUM_LEDS];

// Http Server
AsyncWebServer server(80);

void onUpdateNodes(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
	debugOutln((char *)data);

	xSemaphoreTake(gNetworkSemaphore, 1000 * portTICK_PERIOD_MS);
	gNodes = Node::fromNetworkJson((char *)data, gRootId);
	xSemaphoreGive(gNetworkSemaphore);

	debugOutln("Done with network parsing");

	gNeedUpate = true;

	// debugOutln((char *)data);
	request->send(200);
}

void setup()
{
	gNetworkSemaphore = xSemaphoreCreateBinary();

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
		debugOutln("Connecting to WiFi..");
	}
	debugOutln("Connected to the WiFi network: " + WiFi.localIP().toString());

	// API
	server.on("/api/mode", HTTP_GET, [](AsyncWebServerRequest *request) {
		gMode = (gMode != LedMode::single) ? LedMode::single : LedMode::pride;
		request->send(200, "text/plain", "Success!");
	});

	server.on("/api/delay", HTTP_GET, [](AsyncWebServerRequest *request) {
		gDelay = (gDelay == 100) ? 10 : 100;
		request->send(200, "text/plain", "Success!");
	});

	// server.on(
	// 	"/api/node", HTTP_POST,
	// 	[](AsyncWebServerRequest *request) { request->send(200); },
	// 	[](...) {},
	// 	onUpdateNodes);

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(SPIFFS, "/index.html", String());
	});

	AsyncCallbackJsonWebHandler* nodeHandler = new AsyncCallbackJsonWebHandler("/api/node", [](AsyncWebServerRequest *request, JsonVariant &json) {
 		JsonObject jsonObj = json.as<JsonObject>();

		xSemaphoreTake(gNetworkSemaphore, 1000 * portTICK_PERIOD_MS);
		gNodes = Node::fromNetworkJson(jsonObj, gRootId);
		xSemaphoreGive(gNetworkSemaphore);

		debugOutln("Done with network parsing");

		gNeedUpate = true;

	}, 8192U);
	server.addHandler(nodeHandler);

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

	gContext.startTime = millis();
	gContext.lastUpdate = gContext.startTime;
	gContext.numLeds = NUM_LEDS;
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
	unsigned long currentTime = millis();
	unsigned long delta = gContext.lastUpdate - currentTime;
	gContext.elapsed += (currentTime - gContext.startTime) / 1000.f;

	if (gNodes.find(gRootId) != gNodes.end() && (gNeedUpate || (delta > 200)))
	{
		xSemaphoreTake(gNetworkSemaphore, 10 * portTICK_PERIOD_MS);

		gContext.lastUpdate = currentTime;
		gNeedUpate = false;
		Node::DataRgb rgb;
		for (int i = 0; i < NUM_LEDS; i++)
		{
			LedContext ledCtx;
			ledCtx.id = i;

			gNodes[gRootId]->eval(gContext, ledCtx, "rgb", rgb);

			// debugOut("Eval:");
			// debugOut(rgb.r);
			// debugOut(rgb.g);
			// debugOutln(rgb.b);

			gColors[i].setRGB(rgb.r, rgb.g, rgb.b);
		}

		xSemaphoreGive(gNetworkSemaphore);
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