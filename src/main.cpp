#include <Arduino.h>
#include <FastLED.h>
#include "WiFi.h"
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "Spiffs.h"

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
	single,
	pride,
	off
};

uint16_t gDelay = 100;
LedMode gMode = LedMode::off;

CRGB leds[NUM_LEDS];

// Http Server
AsyncWebServer server(80);

class RequestHandler : public AsyncWebHandler
{
public:
	RequestHandler() {}
	virtual ~RequestHandler() {}

	bool canHandle(AsyncWebServerRequest *request)
	{
		//request->addInterestingHeader("ANY");
		return true;
	}

	void handleRequest(AsyncWebServerRequest *request)
	{
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		response->print("<!DOCTYPE html><html><head><title>Captive Portal</title></head><body>");
		response->print("<p>This is out captive portal front page.</p>");
		response->printf("<p>You were trying to reach: http://%s%s</p>", request->host().c_str(), request->url().c_str());
		response->printf("<p>IP: </p>", WiFi.localIP().toString().c_str());
		response->print("</body></html>");
		request->send(response);
	}
};

void setup()
{
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

	// server.addHandler(new RequestHandler()); //only when requested from AP

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

	SPIFFS.begin();
	server.serveStatic("/", SPIFFS, "/");

	//more handlers...
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
		leds[whiteLed] = CRGB::White;

		// Show the leds (only one of which is set to white, from above)
		FastLED.show();

		// Wait a little bit
		delay(gDelay);

		// Turn our current led back to black for the next loop around
		leds[whiteLed] = CRGB::Black;
	}
}

void loop()
{
	switch (gMode)
	{
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