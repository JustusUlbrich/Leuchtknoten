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

#include "FastLED_RGBW.h"
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
std::shared_ptr<Node::NodeOutput> gRootNode = nullptr;
std::unordered_map<std::string, std::shared_ptr<Node::INode>> gNodes;
bool gNeedUpate = false;

Context gContext;

CRGBW gColors[NUM_LEDS];
CRGBW leds[NUM_LEDS];

// Http Server
AsyncWebServer server(80);

void setup()
{
	gNetworkSemaphore = xSemaphoreCreateBinary();

	for (int i = 0; i < NUM_LEDS; i++)
	{
		gColors[i] = CRGB::White;
		gColors[i].w = 0;
	}

	delay(2000); // sanity delay
	FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>((CRGB *)&leds[0], getRGBWsize(NUM_LEDS));

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

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(SPIFFS, "/index.html", String());
	});

	AsyncCallbackJsonWebHandler *nodeHandler = new AsyncCallbackJsonWebHandler(
		"/api/node", [](AsyncWebServerRequest *request, JsonVariant &json) {
			JsonObject jsonObj = json.as<JsonObject>();

			xSemaphoreTake(gNetworkSemaphore, 1000 * portTICK_PERIOD_MS);
			gNodes = Node::fromNetworkJson(jsonObj, gRootId);
			gRootNode = std::static_pointer_cast<Node::NodeOutput>(gNodes[gRootId]);
			xSemaphoreGive(gNetworkSemaphore);

			debugOutln("Done with network parsing");

			gNeedUpate = true;

			request->send(200, "text/plain", "Success!");
		},
		16384U);
	server.addHandler(nodeHandler);

	AsyncCallbackJsonWebHandler *updateHandler = new AsyncCallbackJsonWebHandler(
		"/api/nodeupdate", [](AsyncWebServerRequest *request, JsonVariant &json) {
			JsonObject jsonObj = json.as<JsonObject>();
			auto nodeId = jsonObj["NodeId"].as<std::string>();
			auto jsonValue = jsonObj["Data"].as<JsonObject>();

			xSemaphoreTake(gNetworkSemaphore, 1000 * portTICK_PERIOD_MS);

			auto toUpdate = gNodes.find(nodeId);
			if (toUpdate != gNodes.end())
				toUpdate->second->updateValue(jsonValue);
			xSemaphoreGive(gNetworkSemaphore);

			debugOutln("Done with update");

			gNeedUpate = true;

			request->send(200, "text/plain", "Success!");
		},
		1024U);
	server.addHandler(updateHandler);

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

		CRGB oldcolor = leds[pixelnumber].toCRGB();
		nblend(oldcolor, newcolor, 64);
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

	if (gRootNode != nullptr && (gNeedUpate || (delta > 0)))
	{
		xSemaphoreTake(gNetworkSemaphore, 10 * portTICK_PERIOD_MS);

		gContext.lastUpdate = currentTime;
		gNeedUpate = false;
		for (int i = 0; i < NUM_LEDS; i++)
		{
			LedContext ledCtx;
			ledCtx.id = i;

			gColors[i] = gRootNode->eval(gContext, ledCtx);

			//debugOut(rgb.r);
			//debugOut(rgb.g);
			//debugOutln(rgb.b);
		}
		debugOut("Eval End");

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