#ifndef NordicBLE_h
#define NordicBLE_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "SerialParser.h"

#define NRF_BAUD_RATE 115200
#define Debug Serial

class NordicBLE
{
private:
	#ifdef USE_HARDWARE_SERIAL
		HardwareSerial *serial;
	#else
		SoftwareSerial *serial;
	#endif
	SerialParser *parser;
	void send_command(byte type, byte *data, int size);
public:
	#ifdef USE_HARDWARE_SERIAL
		NordicBLE(HardwareSerial &serial);
	#else
		NordicBLE(SoftwareSerial &serial);
	#endif
	// call this in loop()
	void tick();
	// debug flg
	bool debug = false;
	//// Softdevice Commands ////
	void sd_ble_enable(uint8_t service_changed = 0, uint32_t attr_tab_size = 0);
	void sd_ble_gap_scan_start();
	// event handler
	void (*on_event)(SerialParser::CommandData &data);
};

#endif
