#include "CommandParser.h"

// initalize
#ifdef USE_HARDWARE_SERIAL
	CommandParser::CommandParser(HardwareSerial *serial)
		:serial(serial) {
	}
#else
	CommandParser::CommandParser(SoftwareSerial *serial)
		:serial(serial) {
	}
#endif

//
void CommandParser::tick() {
	#ifndef USE_HARDWARE_SERIAL
	if (serial->overflow()) {
		// TODO: fix this!
		Debug.println(F("\nSoftwareSerial overflow!"));
	}
	#endif
	while (serial->available()) {
		char c = serial->read();
		// skip broken data
		if (broken) {
			pos = 0;
			break;
		}
		NordicBLE::CommandData &data = dataBuff[dataIn];
		switch (pos++) {
			// packet length: 2byte
			case 0:
				data.len = c;
				break;
			case 1:
				data.len = data.len | (c << 8);
				// impossible value is broken data...
				if (data.len < 2 || data.len > 50) {
					broken = true;
				}
				break;
			// type: 0:command 1:response 2:event
			case 2:
				data.type = c;
				// impossible value is broken data...
				if (data.type < 0 || data.type > 2) {
					broken = true;
				}
				break;
			// command
			case 3:
				data.command = c;
				break;
			// data
			default:
				if (data.type == 2 && pos == 5) {
					continue;
				}
				byte *p = data.data;
				int ofs = 2;
				if (data.type == 2) {
					ofs = 3;
				}
				p += (pos - ofs - 3);
				*p = c;
				if (pos > data.len + 1) {
					// end of data
					data.len -= ofs;
					pos = 0;
					dataCount++;
					dataIn++;
					dataIn %= BUFF_SIZE;
					// command buffer is overflow!
					if (dataIn == dataOut) {
						Debug.println(F("\nOverflow!"));
						delay(100);
						exit(1);
					}
				}
				break;
		}
	}
	broken = false;
}

bool CommandParser::getCommand(NordicBLE::CommandData &out) {
	if (dataCount == 0) return false;
	memcpy(&out, &dataBuff[dataOut++], sizeof(NordicBLE::CommandData));
	dataOut %= BUFF_SIZE;
	dataCount--;
	return true;
}
