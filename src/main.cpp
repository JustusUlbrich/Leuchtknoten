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
const char* ssid = "PrettyFlyForAWifi";
const char* password = "***REMOVED***";

SemaphoreHandle_t gNetworkSemaphore;

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

	delay(500); // sanity delay
	FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>((CRGB*)&leds[0], gLedSize);

	FastLED.setBrightness(BRIGHTNESS);

	Serial.begin(9600);

	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		debugOutln("Connecting to WiFi..");
	}
	debugOutln("Connected to the WiFi network: " + WiFi.localIP().toString());

	// -------
	// API
	// TODO: Cleanup and move to new file
	// TODO: Cleanup endpoints naming :(

	server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(SPIFFS, "/index.html", String());
		});

	AsyncCallbackJsonWebHandler* nodeHandler = new AsyncCallbackJsonWebHandler(
		"/api/node", [](AsyncWebServerRequest* request, JsonVariant& json) {
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

	AsyncCallbackJsonWebHandler* updateHandler = new AsyncCallbackJsonWebHandler(
		"/api/nodeupdate", [](AsyncWebServerRequest* request, JsonVariant& json) {
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

	// --------
	// SAVE
	AsyncCallbackJsonWebHandler* storeHandler = new AsyncCallbackJsonWebHandler(
		"/api/nodestore", [](AsyncWebServerRequest* request, JsonVariant& json) {
			JsonObject jsonObj = json.as<JsonObject>();
			auto networkPath = "/net/" + jsonObj["name"].as<std::string>();
			auto network = jsonObj["network"].as<std::string>();

			debugOut("Storing network: ");
			debugOutln(networkPath.c_str());

			auto file = SPIFFS.open(networkPath.c_str(), "w");
			file.write((uint8_t*)(network.c_str()), network.size());
			file.close();

			request->send(200, "text/plain", "Success!");
		},
		16384U);
	server.addHandler(storeHandler);

	// --------
	// DELETE
	server.on("/api/nodedelete", HTTP_DELETE, [](AsyncWebServerRequest* request) {
		AsyncWebParameter* p = request->getParam(0);
		if (p != nullptr && p->name() == "name")
		{
			auto networkPath = "/net/" + p->value();
			if (SPIFFS.exists(networkPath))
			{
				auto removed = SPIFFS.remove(networkPath.c_str());

				if (removed)
					request->send(200, "text/plain", "Removed");
				else
					request->send(404, "text/plain", "Could not delete Network :(");
				return;
			}
		}

		request->send(404, "text/plain", "Network not found :(");
		});

	// --------
	// LOAD
	server.on("/api/nodeload", HTTP_GET, [](AsyncWebServerRequest* request) {
		AsyncWebParameter* p = request->getParam(0);
		if (p != nullptr && p->name() == "name")
		{
			auto networkPath = "/net/" + p->value();
			if (SPIFFS.exists(networkPath))
			{
				auto file = SPIFFS.open(networkPath.c_str(), "r");
				auto fileContent = file.readString();
				file.close();

				request->send(200, "text/json", fileContent);
				return;
			}
		}

		request->send(404, "text/plain", "Network not found :(");
		});

	server.on("/api/nodelist", HTTP_GET, [](AsyncWebServerRequest* request) {
		File dir = SPIFFS.open("/net");
		if (!dir || !dir.isDirectory())
			request->send(404, "text/plain", "Network directory not found :(");

		// filename is <32 char. So this is >10 files
		char result[400] = "{\"nodes\":[";
		File file = dir.openNextFile();
		while (file)
		{
			strcat(result, "\"");
			strcat(result, &file.name()[5]);
			strcat(result, "\"");
			file = dir.openNextFile();
			if (file)
				strcat(result, ",");
		};
		strcat(result, "]}");
		request->send(200, "text/json", result);
		});

	// Web
	SPIFFS.begin();
	server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html.gz");

	// Options default handler
	server.onNotFound([](AsyncWebServerRequest* request) {
		if (request->method() == HTTP_OPTIONS) {
			request->send(200);
		}
		else {
			request->send(404);
		} });

	// CORS
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "content-type");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "*");
	server.begin();

	gContext.elapsed = 0.f;
	gContext.lastUpdate = millis();
	gContext.numLeds = NUM_LEDS;

	// RTP Midi
	MIDI.begin();

	AppleMIDI.setHandleConnected([](const APPLEMIDI_NAMESPACE::ssrc_t& ssrc, const char* name) {
		debugOutln("Connected to session");
		debugOutln(name);
		});
	AppleMIDI.setHandleDisconnected([](const APPLEMIDI_NAMESPACE::ssrc_t& ssrc) {
		debugOutln("Disconnected session");
		});

	MIDI.setHandleNoteOn([](byte channel, byte note, byte velocity) {
		debugOut("Midi note - ch");
		debugOut(channel);
		debugOut(" - note ");
		debugOut(note);
		debugOut(" - velo ");
		debugOutln(velocity);
		gContext.activeNotes.push_back(MidiNote{ channel, note, velocity });
		// gContext.midiByNote[note] = true;
		});
	MIDI.setHandleNoteOff([](byte channel, byte note, byte velocity) {
		debugOutln("Midi note off");
		// gContext.midiByNote[note] = false;
		});
}

void loop()
{
	// Listen to incoming notes
	MIDI.read();

	unsigned long currentTime = millis();
	gContext.elapsed = currentTime;
	gContext.delta = (currentTime - gContext.lastUpdate);

	if (gRootNode != nullptr && (gNeedUpate || (gContext.delta > 0)))
	{
		xSemaphoreTake(gNetworkSemaphore, 10 * portTICK_PERIOD_MS);

		gContext.lastUpdate = currentTime;
		gNeedUpate = false;

		LedContext ledCtx;
		ledCtx.id = -1;
		for (auto& node : gNodes)
			node.second->preEval(gContext, ledCtx);

		for (int i = 0; i < NUM_LEDS; i++)
		{
			ledCtx.id = i;
			gColors[i] = gRootNode->eval(gContext, ledCtx);
		}

		for (auto& node : gNodes)
			node.second->postEval(gContext, ledCtx);

		gContext.activeNotes.clear();
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