#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "ssid.h"
const char *ssid = STASSID;
const char *password = STAPSK;

#include <FastLED.h>

#define NUM_LEDS 66
CRGB leds[NUM_LEDS];

#define LED_PIN1 2
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define NUM_SPARKS 25 // max number (could be NUM_LEDS / 2);
float sparkPos[NUM_SPARKS];
float sparkVel[NUM_SPARKS];
float sparkCol[NUM_SPARKS];

#define GLOBAL_DELAY 5
#define FLARE_HIGHT 50

void flare();
void explode02();
void bang();

// For mirroring strips, all the "special" stuff happens just in setup.  We
// just addLeds multiple times, once for each strip
void setup()
{
  FastLED.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // OTA
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]()
                     {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type); });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    } });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  ArduinoOTA.handle();

  flare();
  delay(50);
  bang();
  explode02();

  FastLED.clear();
  FastLED.show();

  delay(random16(500, 3000));
}

void flare()
{
  float gravity = -.0050; // m/s/s
  float flarePos = 0;
  float flareVel = 0.70;
  float brightness = 1;

  // launch
  FastLED.clear();
  while (flareVel >= 0.0)
  {
    leds[int(flarePos)] = CHSV(0, 0, int(brightness * 255));
    FastLED.show();
    FastLED.clear();
    flarePos += flareVel;
    flareVel += gravity;
    // brightness *= .997;
    // Serial.print(int(flarePos)); Serial.print("\n");

    delay(GLOBAL_DELAY);
  }
  // Serial.print("flare end\n");
}

void bang()
{
  // FastLED.clear();

  const int bangsize = 2;

  for (int i = -bangsize; i < bangsize; ++i)
  {
    leds[FLARE_HIGHT + i] = CHSV(0, 0, 255);
  }

  FastLED.show();
  delay(30);
  FastLED.clear();
  FastLED.show();
}

void explode02()
{
  // int nSparks = flarePos / 2; // works out to look about right
  float flarePos = FLARE_HIGHT;
  int nSparks = 8;

  float sparkTime = 255.0;

  uint8_t hue = random8();
  Serial.print(hue);
  Serial.print("\n");

  // initialize sparks
  for (int i = 0; i < nSparks; i++)
  {
    sparkPos[i] = flarePos;
    sparkVel[i] = (float(random16(0, 10000)) / 50000.0); // 0 to 1
    sparkCol[i] = constrain(sparkVel[i] * 500, 0, 255);
  }

  while (sparkTime > 1.0)
  {
    FastLED.clear();
    // sparkTime *= .95;
    sparkTime -= 1;
    // Serial.print(sparkTime); Serial.print("\n");

    for (int i = 0; i < nSparks; i++)
    {
      sparkPos[i] += sparkVel[i];
      sparkPos[i] = constrain(sparkPos[i], 51, NUM_LEDS);
      sparkCol[i] *= .97;
      sparkCol[i] = constrain(sparkCol[i], 0, 255); // red cross dissolve

      float brightness = 255;
      if (sparkTime < 150)
      {
        brightness = 255 * sparkTime / 150;
      }

      leds[int(sparkPos[i])] = CHSV(hue, 255, brightness);
    }
    FastLED.show();
    FastLED.delay(GLOBAL_DELAY);
  }
}

void explode01()
{
  // int nSparks = flarePos / 2; // works out to look about right
  float flarePos = FLARE_HIGHT + 1;
  int nSparks = 7;

  // initialize sparks
  for (int i = 0; i < nSparks; i++)
  {
    sparkPos[i] = flarePos;
    sparkVel[i] = (float(random16(0, 10000)) / 40000.0); // 0 to 1
    sparkCol[i] = constrain(sparkVel[i] * 500, 0, 255);
  }

  sparkCol[0] = 255; // this will be our known spark
  float c1 = 120;
  float c2 = 50;

  while (sparkCol[0] > c2 / 128)
  { // as long as our known spark is lit, work with all the sparks
    FastLED.clear();
    for (int i = 0; i < nSparks; i++)
    {
      sparkPos[i] += sparkVel[i];
      sparkPos[i] = constrain(sparkPos[i], FLARE_HIGHT + 1, NUM_LEDS);
      sparkCol[i] *= .99;
      sparkCol[i] = constrain(sparkCol[i], 0, 255); // red cross dissolve
      if (sparkCol[i] > c1)
      { // fade white to yellow
        leds[int(sparkPos[i])] = CRGB(255, 255, (255 * (sparkCol[i] - c1)) / (255 - c1));
      }
      else if (sparkCol[i] < c2)
      { // fade from red to black
        leds[int(sparkPos[i])] = CRGB((255 * sparkCol[i]) / c2, 0, 0);
      }
      else
      { // fade from yellow to red
        leds[int(sparkPos[i])] = CRGB(255, (255 * (sparkCol[i] - c2)) / (c1 - c2), 0);
      }
    }
    FastLED.show();

    FastLED.delay(10);
  }
}
