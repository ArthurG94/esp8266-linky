#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#ifndef LINKY_DEF
#define LINKY_DEV ;

class Linky
{

public:
	Linky();

	void begin();
	void loop();

	String ADCO;
	String OPTARIF;
	byte ISOUSC;
	long BASE;
	String PTEC;
	int IINST;
	int IMAX;
	int PAPP;
	String HHPHC;
	String MOTDETAT;

private:
	SoftwareSerial serial;
	byte data[25];
	byte dataLength = 0;


};

#endif
