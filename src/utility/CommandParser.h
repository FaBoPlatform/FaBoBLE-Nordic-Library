#ifndef CommandParser_h
#define CommandParser_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "NordicBLE.h"

// #define USE_HARDWARE_SERIAL

// command buffer size
#define BUFF_SIZE 10
#define Debug Serial

class CommandParser
{
private:
	#ifdef USE_HARDWARE_SERIAL
		HardwareSerial *serial;
	#else
		SoftwareSerial *serial;
	#endif
	int pos = 0;
	bool eod = false;
	NordicBLE::CommandData dataBuff[BUFF_SIZE];
	int dataCount = 0;
	int dataIn = 0;
	int dataOut = 0;
	bool broken = false;

public:
	#ifdef USE_HARDWARE_SERIAL
		CommandParser(HardwareSerial *serial);
	#else
		CommandParser(SoftwareSerial *serial);
	#endif
	void tick();
	bool getCommand(NordicBLE::CommandData &out);
};

#endif
