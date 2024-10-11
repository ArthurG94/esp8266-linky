
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <uri/UriBraces.h>
#include <WiFiManager.h>

#include <Linky.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

WiFiClient espClient;
ESP8266WebServer server(80);
Linky linky;
Adafruit_NeoPixel led(1, 14, NEO_GRB);

void setup()
{
	Serial.begin(9600);
	WiFiManager wifiManager;

	led.begin();
	led.setPixelColor(0, 255, 0, 255);
	led.show();

	if (!wifiManager.autoConnect("Linky AP", "123456789"))
	{
		WiFi.hostname("linky");
		ESP.reset();
		delay(1000);
	}

	server.on(UriBraces("/"), []() { server.send(200, "application/json", "{\"ADCO\":\"" + linky.ADCO + "\"," + 
	"\"OPTARIF\":\"" + linky.OPTARIF + "\"," + 
	"\"ISOUSC\":" + String(linky.ISOUSC) + "," + 
	"\"BASE\":" + String(linky.BASE) + "," + 
	"\"PTEC\":\"" + linky.PTEC + "\"," + 
	"\"IINST\":" + String(linky.IINST) + "," + 
	"\"IMAX\":" + String(linky.IMAX) + "," + 
	"\"PAPP\":" + String(linky.PAPP) + "," + 
	"\"HHPHC\":\"" + linky.HHPHC + "\"," + 
	"\"MOTDETAT\":\"" + linky.MOTDETAT + "\"}"); });
	server.on(UriBraces("/ADCO"), []() { server.send(200, "text/plain", linky.ADCO); });
	server.on(UriBraces("/OPTARIF"), []() { server.send(200, "text/plain", linky.OPTARIF); });
	server.on(UriBraces("/ISOUSC"), []() { server.send(200, "text/plain", String(linky.ISOUSC)); });
	server.on(UriBraces("/BASE"), []() { server.send(200, "text/plain", String(linky.BASE)); });
	server.on(UriBraces("/PTEC"), []() { server.send(200, "text/plain", linky.PTEC); });
	server.on(UriBraces("/IINST"), []() { server.send(200, "text/plain", String(linky.IINST)); });
	server.on(UriBraces("/IMAX"), []() { server.send(200, "text/plain", String(linky.IMAX)); });
	server.on(UriBraces("/PAPP"), []() { server.send(200, "text/plain", String(linky.PAPP)); });
	server.on(UriBraces("/HHPHC"), []() { server.send(200, "text/plain", linky.HHPHC); });
	server.on(UriBraces("/MOTDETAT"), []() { server.send(200, "text/plain", linky.MOTDETAT); });

	server.begin();
	linky.begin();
}

unsigned long oldTime = 0;
bool ledON = true;
bool blink = false;



void loop()
{

	if (millis() - oldTime >= 1000)
	{
		ledON = !ledON;
		oldTime = millis();
	}

	
	float ratio = ((float)linky.IINST / linky.ISOUSC); // Abonnement 6Kw

	if (ratio > 1)
	{ // Possible en cas de dépassement de l'abonnement.
		ratio = 1;
	}

	byte red = 30 * ratio;
	byte green = 30 - red;
	byte blue = 0;

	blink = ratio >= 1;

	if ((ledON && blink) || !blink)
	{
		led.setPixelColor(0, red, green, blue);
	}
	else
	{
		led.setPixelColor(0, 0, 0, 0);
	}

	led.show();
	linky.loop();
	server.handleClient();
}
