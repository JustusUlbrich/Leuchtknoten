#include <Arduino.h>
#include "freertos/FreeRTOS.h"

#include <FastLED.h>
#include "WiFi.h"
#include <WiFiClientSecure.h>
#include <AsyncTCP.h>
#include <AsyncJson.h>

#include "ESPAsyncWebServer.h"
#include "Spiffs.h"

#include <unordered_map>
#include <string>

#include "FastLED_RGBW.h"
#include "defines.h"

// Spotify
#include "spotify/ArduinoSpotify.h"
#include "spotify/ArduinoSpotifyCert.h"
WiFiClientSecure secureWifiClient;
std::shared_ptr<ArduinoSpotify> gSpotify = nullptr;

// MIDI
#define SerialMon Serial
#define APPLEMIDI_DEBUG SerialMon
#include <AppleMIDI.h>

APPLEMIDI_CREATE_DEFAULTSESSION_INSTANCE();

// Nodes
#include "NodeNetwork/api/INode.hpp"
#include <ArduinoJson.h>
// #include "json.hpp"
#include "NodeNetwork/Generators.hpp"

SemaphoreHandle_t gNetworkSemaphore;

std::string gRootId = "";
std::shared_ptr<Node::NodeOutput> gRootNode = nullptr;
std::unordered_map<std::string, std::shared_ptr<Node::INode>> gNodes;
bool gNeedUpate = false;

Context gContext;

// Http Server
AsyncWebServer server(80);

boolean initConfig()
{
	File file = SPIFFS.open(CONFIG_FILE);
	if (!file) {
		debugOutln("Failed to read config file");
		return false;
	}
	StaticJsonDocument<1024> doc;

	DeserializationError error = deserializeJson(doc, file);
	if (error) {
		debugOutln("Failed to deserialize config file, using default configuration");
		debugOutln(error.c_str());
		return false;
	}

	JsonObject leds = doc["leds"];
	gContext.config.leds.count = leds["count"] | 16;
	gContext.config.leds.brightness = leds["brightness"] | 255;

	gContext.config.wifi.ssid = doc["wifi"]["ssid"] | "";
	gContext.config.wifi.key = doc["wifi"]["key"] | "";

	debugOutln(gContext.config.wifi.ssid.c_str());
	debugOutln(gContext.config.wifi.key.c_str());

	JsonObject spotify = doc["spotify"];
	gContext.config.spotify.enabled = spotify["enabled"] | false;
	gContext.config.spotify.updateRateMs = spotify["update_rate_ms"] | 10000;
	gContext.config.spotify.market = spotify["market"] | "DE";
	gContext.config.spotify.clientId = spotify["client_id"] | "";
	gContext.config.spotify.clientSecret = spotify["client_secret"] | "";
	gContext.config.spotify.refreshToken = spotify["refresh_token"] | "";

	gContext.config.midi.enabled = doc["midi"]["enabled"] | true;

	return true;
}

void setup()
{
	delay(500); // sanity delay
	Serial.begin(9600);
	gNetworkSemaphore = xSemaphoreCreateBinary();

	SPIFFS.begin();

	// Init config from file
	initConfig();
	auto& config = gContext.config;

	// Init Context
	gContext.ledBuffer.resize(config.leds.count);
	gContext.colorBuffer.resize(config.leds.count);
	for (int i = 0; i < config.leds.count; i++)
	{
		gContext.colorBuffer[i] = CRGB::White;
		gContext.colorBuffer[i].w = 0;
	}

	// Init LED
	const auto ledBuffSize = LED_USE_RGBW ? getRGBWsize(config.leds.count) : config.leds.count;
	const auto ledBuffer = (CRGB*)gContext.ledBuffer.data();
	FastLED.addLeds<WS2812, LED_GPIO_PIN, LED_COLOR_ORDER>(ledBuffer, ledBuffSize);
	FastLED.setBrightness(config.leds.count);


	// Init Wifi
	WiFi.begin(config.wifi.ssid.c_str(), config.wifi.key.c_str());
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
		"/api/node", [&config](AsyncWebServerRequest* request, JsonVariant& json) {
			JsonObject jsonObj = json.as<JsonObject>();

			xSemaphoreTake(gNetworkSemaphore, 1000 * portTICK_PERIOD_MS);
			gNodes = Node::fromNetworkJson(jsonObj, config, gRootId);
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

	// RTP Midi
	if (config.midi.enabled)
	{
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

	// Spotify
	if (config.spotify.enabled) {
		secureWifiClient.setCACert(spotify_server_cert);
		gSpotify = std::make_shared<ArduinoSpotify>(
			secureWifiClient,
			config.spotify.clientId.c_str(),
			config.spotify.clientSecret.c_str(),
			config.spotify.refreshToken.c_str()
			);
		debugOutln("Refreshing Access Tokens");
		if (!gSpotify->refreshAccessToken())
		{
			debugOutln("Failed to get access tokens");
		}
	}
}

void loop()
{
	const auto& config = gContext.config;

	// Listen to incoming notes
	if (config.midi.enabled)
		MIDI.read();

	unsigned long currentTime = millis();
	gContext.elapsed = currentTime;
	gContext.delta = (currentTime - gContext.lastUpdate);

	if (config.spotify.enabled && currentTime > gContext.lastSpotifyUpdate + config.spotify.updateRateMs) {
		gContext.lastSpotifyUpdate = currentTime;
		CurrentlyPlaying currentlyPlaying = gSpotify->getCurrentlyPlaying(config.spotify.market.c_str());
		gContext.spotifyDuration = currentlyPlaying.duraitonMs;
		gContext.spotifyProgress = currentlyPlaying.progressMs;
		if (gContext.spotifyDuration > 0) {
			debugOut(gContext.spotifyProgress);
			debugOut(" / ");
			debugOutln(gContext.spotifyDuration);
			debugOut(" % ");
			debugOutln(100.f * gContext.spotifyProgress / (float)gContext.spotifyDuration);
		}
	}

	if (gRootNode != nullptr && (gNeedUpate || (gContext.delta > 0)))
	{
		xSemaphoreTake(gNetworkSemaphore, 10 * portTICK_PERIOD_MS);

		gContext.lastUpdate = currentTime;
		gNeedUpate = false;

		LedContext ledCtx;
		ledCtx.id = -1;
		for (auto& node : gNodes)
			node.second->preEval(gContext, ledCtx);

		for (int i = 0; i < config.leds.count; i++)
		{
			ledCtx.id = i;
			gContext.colorBuffer[i] = gRootNode->eval(gContext, ledCtx);
		}

		for (auto& node : gNodes)
			node.second->postEval(gContext, ledCtx);

		gContext.activeNotes.clear();

		xSemaphoreGive(gNetworkSemaphore);

		for (int i = 0; i < config.leds.count; i++)
		{
#ifdef LED_USE_RGBW
			gContext.ledBuffer[i] = gContext.colorBuffer[i];
#else
			gContext.ledBuffer[i] = gContext.colorBuffer[i].toCRGB();
#endif
		}
	}
	FastLED.show();
}