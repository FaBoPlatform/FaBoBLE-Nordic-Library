#include "fabo-nordic.h"

// initalize
#ifdef USE_HARDWARE_SERIAL
	void NordicBLE::init(HardwareSerial &serial, bool debug)
	{
		this->serial = &serial;
		this->debug = debug;
		serial.begin(NRF_BAUD_RATE);
	}
#else
	void NordicBLE::init(SoftwareSerial &serial, bool debug)
	{
		this->serial = &serial;
		this->debug = debug;
		serial.begin(NRF_BAUD_RATE);
	}
#endif

// sd_ble_enable
void NordicBLE::sd_ble_enable(uint8_t service_changed, uint32_t attr_tab_size) {
	byte data[] = {
		0x01, service_changed, 0x00, 0x00, 0x00, 0x00
	};
	memcpy(&data[2], &attr_tab_size, 4);
	this->sendCommand(0x60, data, 6);
}

// Send command
void NordicBLE::sendCommand(byte type, byte *data, int size) {
	// Packet size
	int len = size + 2;
	serial->write(len);
	serial->write(len << 8);
	// type: 0=command
	serial->write((byte)0);
	// Command type
	serial->write(type);
	// Data
	serial->write(data, size);
	// Debug
	if (debug) {
		Debug.println("*sendCommand");
		Debug.print("type:");
		Debug.println(type, HEX);
		Debug.print("data:");
	  for (int i=0; i<size; i++) {
			Debug.print(data[i], HEX);
			if (i<size-1) Debug.print(",");
	  }
		Debug.println("");
	}
}
