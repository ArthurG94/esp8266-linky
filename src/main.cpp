
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <PubSubClient.h>
#include <WiFiManager.h>

#include <Linky.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define MQTT_SERVER "192.168.1.101"
#define MQTT_PORT 1883

WiFiClient espClient;
PubSubClient client(espClient);
Linky linky;
Adafruit_NeoPixel led(1, 14, NEO_GRB);

void setup()
{
	Serial.begin(9600);
	WiFiManager wifiManager;
	led.begin();
	led.show();

	if (!wifiManager.autoConnect("Linky AP", "123456789"))
	{
		WiFi.hostname("linky");
		ESP.reset();
		delay(1000);
	}

	client.setServer(MQTT_SERVER, MQTT_PORT);
	linky.begin();
}

unsigned long oldTime = 0;
bool ledON = true;
bool blinky = false;

String old_ADCO;
String old_OPTARIF;
byte old_ISOUSC;
String old_BASE;
String old_PTEC;
int old_IINST;
int old_IMAX;
int old_PAPP;
String old_HHPHC;
String old_MOTDETAT;

void loop()
{

	if (!client.connected())
	{
		if (!client.connect("linky"))
		{
			delay(1000);
		}
	}
	else
	{

		if (old_ADCO != linky.ADCO)
		{
			client.publish("linky/ADCO", linky.ADCO.c_str());
			old_ADCO = linky.ADCO;
		}
		if (old_OPTARIF != linky.OPTARIF)
		{
			client.publish("linky/OPTARIF", linky.OPTARIF.c_str());
			old_OPTARIF = linky.OPTARIF;
		}
		if (old_ISOUSC != linky.ISOUSC)
		{
			client.publish("linky/ISOUSC", String(linky.ISOUSC).c_str());
			old_ISOUSC = linky.ISOUSC;
		}
		if (old_BASE != linky.BASE)
		{
			client.publish("linky/BASE", linky.BASE.c_str());
			old_BASE = linky.BASE;
		}
		if (old_PTEC != linky.PTEC)
		{
			client.publish("linky/PTEC", linky.PTEC.c_str());
			old_PTEC = linky.PTEC;
		}
		if (old_IINST != linky.IINST)
		{
			client.publish("linky/IINST", String(linky.IINST).c_str());
			old_IINST = linky.IINST;
		}
		if (old_IMAX != linky.IMAX)
		{
			client.publish("linky/IMAX", String(linky.IMAX).c_str());
			old_IMAX = linky.IMAX;
		}
		if (old_PAPP != linky.PAPP)
		{
			client.publish("linky/PAPP", String(linky.PAPP).c_str());
			old_PAPP = linky.PAPP;
		}
		if (old_HHPHC != linky.HHPHC)
		{
			client.publish("linky/HHPHC", linky.HHPHC.c_str());
			old_HHPHC = linky.HHPHC;
		}
		if (old_MOTDETAT != linky.MOTDETAT)
		{
			client.publish("linky/MOTDETAT", linky.MOTDETAT.c_str());
			old_MOTDETAT = linky.MOTDETAT;
		}

		float ratio = ((float)linky.IINST / linky.ISOUSC); // Abonnement 6Kw

		if (ratio > 1)
		{ // Possible en cas de dépassement de l'abonnement.
			ratio = 1;
		}

		byte red = 30 * ratio;
		byte green = 30 - red;
		byte blue = 0;

		blinky = ratio >= 1;

		if (millis() - oldTime >= 500)
		{
			ledON = !ledON;
			oldTime = millis();
		}

		if ((ledON && blinky) || !blinky)
		{
			led.setPixelColor(0, red, green, blue);
		}
		else
		{
			led.setPixelColor(0, 0, 0, 0);
		}
	}

	led.show();
	client.loop();
	linky.loop();
}

// 0	360
// 0	65535
// 100/360*65535