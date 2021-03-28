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

#define SerialMon Serial
#define APPLEMIDI_DEBUG SerialMon
#include <AppleMIDI.h>

APPLEMIDI_CREATE_DEFAULTSESSION_INSTANCE();

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

#ifdef LED_USE_WHITE
CRGBW leds[NUM_LEDS];
uint16_t gLedSize = getRGBWsize(NUM_LEDS);
#else
CRGB leds[NUM_LEDS];
uint16_t gLedSize = NUM_LEDS;
#endif

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
	FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>((CRGB *)&leds[0], gLedSize);

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
			{
				debugOutln("Node update");
				toUpdate->second->updateValue(jsonValue);
			}
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

	// RTP Midi
	MIDI.begin();

	AppleMIDI.setHandleConnected([](const APPLEMIDI_NAMESPACE::ssrc_t &ssrc, const char *name) {
		debugOutln("Connected to session");
		debugOutln(name);
	});
	AppleMIDI.setHandleDisconnected([](const APPLEMIDI_NAMESPACE::ssrc_t &ssrc) {
		debugOutln("Disconnected session");
	});

	MIDI.setHandleNoteOn([](byte channel, byte note, byte velocity) {
		debugOut("Midi note - ch");
		debugOut(channel);
		debugOut(" - note ");
		debugOut(note);
		debugOut(" - velo ");
		debugOutln(velocity);
		gContext.midiByNote[note] = true;
	});
	MIDI.setHandleNoteOff([](byte channel, byte note, byte velocity) {
		debugOutln("Midi note off");
		gContext.midiByNote[note] = false;
	});

	gContext.lastUpdate = millis();
}

void loop()
{
	// Listen to incoming notes
	MIDI.read();

	unsigned long currentTime = millis();
	unsigned long delta = (gContext.lastUpdate - currentTime) / 1000000L;
	gContext.elapsed += (currentTime - gContext.startTime) / 1000000L;

	if (gRootNode != nullptr && (gNeedUpate || (delta > 0)))
	{
		xSemaphoreTake(gNetworkSemaphore, 10 * portTICK_PERIOD_MS);

		gContext.lastUpdate = currentTime;
		gNeedUpate = false;

		LedContext ledCtx;
		ledCtx.id = -1;
		for (auto &node : gNodes)
			node.second->preEval(delta, gContext, ledCtx);

		for (int i = 0; i < NUM_LEDS; i++)
		{
			ledCtx.id = i;
			gColors[i] = gRootNode->eval(gContext, ledCtx);
		}

		for (auto &node : gNodes)
			node.second->postEval(delta, gContext, ledCtx);

		// debugOut("Eval End");

		xSemaphoreGive(gNetworkSemaphore);

		for (int i = 0; i < NUM_LEDS; i++)
		{
#ifdef LED_USE_WHITE
			leds[i] = gColors[i];
#else
			leds[i] = gColors[i].toCRGB();
#endif
		}
	}
	FastLED.show();
}