#include "Arduino.h"

// #define USE_HARDWARE_SERIAL
#define NRF_BAUD_RATE 38400
#define Debug Serial

#ifndef USE_HARDWARE_SERIAL
	#include <SoftwareSerial.h>
#endif

class SerialParser
{
public:
	struct Data {
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
	int data_len = 0;
	bool eod = false;
	Data data = {0};

public:
	#ifdef USE_HARDWARE_SERIAL
		SerialParser(HardwareSerial *serial)
			:serial(serial) {
		}
	#else
		SerialParser(SoftwareSerial *serial)
			:serial(serial) {
		}
	#endif
	void tick() {
		if (serial->overflow()) {
	    Serial.println("\nSoftwareSerial overflow!");
	    delay(100);
	    exit(1);
	  }
		while (serial->available()) {
			if (eod) break;
			char c = serial->read();
			switch (pos++) {
				case 0:
					data.len = c;
					break;
				case 1:
					data.len = data.len | (c << 8);
					data.len -= 2;
					break;
				case 2:
					data.type = c;
					break;
				case 3:
					data.command = c;
					break;
				default:
					byte *p = data.data;
					p = p + pos - 4;
					*p = c;
					if (pos >= data.len +4) {
						eod = true;
					}
					break;
			}
		}
	}
	bool read_line(Data *out) {
		if (!eod) return false;
		memcpy(out, &data, sizeof(Data));
		pos = 0;
		eod = false;
		return true;
	}
};

class NordicBLE
{
private:
	#ifdef USE_HARDWARE_SERIAL
		HardwareSerial *serial;
	#else
		SoftwareSerial *serial;
	#endif
	bool debug = false;
	SerialParser *parser;
	void send_command(byte type, byte *data, int size);
public:
	#ifdef USE_HARDWARE_SERIAL
		NordicBLE(HardwareSerial &serial, bool debug = false);
	#else
		NordicBLE(SoftwareSerial &serial, bool debug = false);
	#endif
	void sd_ble_enable(uint8_t service_changed = 0, uint32_t attr_tab_size = 0);
	void sd_ble_gap_scan_start();
	void tick();
};
