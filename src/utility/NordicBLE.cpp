#include "NordicBLE.h"

// initalize
#ifdef USE_HARDWARE_SERIAL
	NordicBLE::NordicBLE(HardwareSerial &serial)
		:serial(&serial)
	{
		serial.begin(NRF_BAUD_RATE);
		parser = new SerialParser(&serial);
	}
#else
	NordicBLE::NordicBLE(SoftwareSerial &serial)
		:serial(&serial)
	{
		serial.begin(NRF_BAUD_RATE);
		parser = new SerialParser(&serial);
	}
#endif


// tick
void NordicBLE::tick() {
	parser->tick();
	SerialParser::CommandData data;
	if (parser->getCommand(&data)) {
		// Debug print
		if (debug) {
			Debug.println("\n*Response");
			Serial.write("typ:");
			Serial.println(data.type, HEX);
			Serial.write("cmd:");
			Serial.println(data.command, HEX);
			Serial.write("len:");
			Serial.println(data.len, DEC);
			Serial.write("dat:");
			for (int i=0; i<data.len; i++) {
				Serial.print(data.data[i], HEX);
				if (i == data.len-1) {
					Serial.println("");
				} else {
					Serial.print(",");
				}
		  }
		}
	}
}

// sd_ble_enable
void NordicBLE::sd_ble_enable(uint8_t service_changed, uint32_t attr_tab_size) {
	byte data[] = {
		0x01, service_changed, 0x00, 0x00, 0x00, 0x00
	};
	memcpy(&data[2], &attr_tab_size, 4);
	this->send_command(0x60, data, 6);
}

// sd_ble_gap_scan_start
void NordicBLE::sd_ble_gap_scan_start() {
	byte data[] = {
		0x01, 0x00, 0x00, 0xa0, 0x00, 0x50, 0x00, 0x00, 0x00
	};
	this->send_command(0x86, data, 9);
}

 #pragma mark - Private

// Send command
void NordicBLE::send_command(byte cmd, byte *data, int size) {
	int len = size + 2;
	// Debug print
	if (debug) {
		Debug.println("\n*Send command");
		Debug.print("len:");
		Debug.println(len | (len << 8) & 0xFF, HEX);
		Debug.print("cmd:");
		Debug.println(cmd, HEX);
		Debug.print("dat:");
		for (int i=0; i<size; i++) {
			Debug.print(data[i], HEX);
			if (i<size-1) Debug.print(",");
		}
		Debug.println("");
	}
	// Packet size
	serial->write(len);
	serial->write(len << 8);
	// type: 0=command
	serial->write((byte)0);
	// Command type
	serial->write(cmd);
	// Data
	serial->write(data, size);
}
