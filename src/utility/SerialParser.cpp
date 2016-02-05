#include "SerialParser.h"

// initalize
#ifdef USE_HARDWARE_SERIAL
	SerialParser::SerialParser(HardwareSerial *serial)
		:serial(serial) {
	}
#else
	SerialParser::SerialParser(SoftwareSerial *serial)
		:serial(serial) {
	}
#endif

//
void SerialParser::tick() {
	#ifndef USE_HARDWARE_SERIAL
	if (serial->overflow()) {
		// TODO: fix this!
		Debug.println("\nSoftwareSerial overflow!");
	}
	#endif
	if (serial->available()) {
		char c = serial->read();
		// skip broken data
		if (broken) {
			pos = 0;
			return;
		}
		CommandData &data = dataBuff[dataIn];
		switch (pos++) {
			// packet length: 2byte
			case 0:
				data.len = c;
				break;
			case 1:
				data.len = data.len | (c << 8);
				data.len -= 2;
				// impossible value is broken data...
				if (data.len < 0 || data.len > 50) {
					broken = true;
				}
				break;
			// type: 0:command 1:response 2:event
			case 2:
				data.type = c;
				break;
			// command
			case 3:
				data.command = c;
				break;
			// data
			default:
				byte *p = data.data;
				p += pos - 5;
				*p = c;
				if (pos > data.len + 3) {
					// end of data
					pos = 0;
					dataCount++;
					dataIn++;
					dataIn %= BUFF_SIZE;
					// command buffer overflow!
					if (dataIn == dataOut) {
						Debug.println("\nOverflow!");
						delay(100);
						exit(1);
					}
				}
				break;
		}
	} else {
		broken = false;
	}
}

bool SerialParser::getCommand(CommandData *out) {
	if (dataCount == 0) return false;
	memcpy(out, &dataBuff[dataOut++], sizeof(CommandData));
	dataOut %= BUFF_SIZE;
	dataCount--;
	return true;
}
