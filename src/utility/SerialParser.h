#ifndef SerialParser_h
#define SerialParser_h

#include <Arduino.h>
#include <SoftwareSerial.h>

// #define USE_HARDWARE_SERIAL

// command buffer size
#define BUFF_SIZE 10
#define Debug Serial

class SerialParser
{
public:
	struct CommandData {
		int len;
		byte type;
		byte command;
		byte data[50];
	};

private:
	#ifdef USE_HARDWARE_SERIAL
		HardwareSerial *serial;
	#else
		SoftwareSerial *serial;
	#endif
	int pos = 0;
	bool eod = false;
	CommandData dataBuff[BUFF_SIZE];
	int dataCount = 0;
	int dataIn = 0;
	int dataOut = 0;
	bool broken = false;

public:
	#ifdef USE_HARDWARE_SERIAL
		SerialParser(HardwareSerial *serial);
	#else
		SerialParser(SoftwareSerial *serial);
	#endif
	void tick();
	bool getCommand(CommandData *out);
};

#endif
