#include "NordicBLE.h"
#include "CommandParser.h"

// initalize
#ifdef USE_HARDWARE_SERIAL
	NordicBLE::NordicBLE(HardwareSerial &serial)
		:serial(&serial)
	{
		serial.begin(NRF_BAUD_RATE);
		parser = new CommandParser(&serial);
	}
#else
	NordicBLE::NordicBLE(SoftwareSerial &serial)
		:serial(&serial)
	{
		serial.begin(NRF_BAUD_RATE);
		parser = new CommandParser(&serial);
	}
#endif


// tick
void NordicBLE::tick() {
	parser->tick();
	CommandData data;
	if (parser->getCommand(data)) {
		// Debug print
		#ifdef DEBUG_PRINT
		if (debug) {
			Debug.println(F("\n*Response"));
			Debug.print(F("typ:"));
			Debug.println(data.type, HEX);
			Debug.print(F("cmd:"));
			Debug.println(data.command, HEX);
			Debug.print(F("len:"));
			Debug.println(data.len, DEC);
			Debug.print(F("dat:"));
			for (int i=0; i<data.len; i++) {
				Debug.print(data.data[i], HEX);
				if (i == data.len-1) {
					Debug.println(F(""));
				} else {
					Debug.print(F(","));
				}
		  }
		}
		#endif
		// send event to handler
		if (handler) {
			handler->nrfReceive(data);
		}
	}
}

// sd_ble_enable
const uint8_t NordicBLE::sd_ble_enable_cmd = 0x60;
void NordicBLE::sd_ble_enable(uint8_t service_changed, uint32_t attr_tab_size) {
	byte data[] = {
		0x01, service_changed, 0x00, 0x00, 0x00, 0x00
	};
	memcpy(&data[2], &attr_tab_size, 4);
	this->send_command(sd_ble_enable_cmd, data, 6);
}

// sd_ble_uuid_vs_add
const uint8_t NordicBLE::sd_ble_uuid_vs_add_cmd = 0x63;
void NordicBLE::sd_ble_uuid_vs_add(uint8_t *uuid) {
	byte data[] = {
		0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x01
	};
	memcpy(&data[1], uuid, 16);
	this->send_command(sd_ble_uuid_vs_add_cmd, data, 18);
}

// sd_ble_version_get
const uint8_t NordicBLE::sd_ble_version_get_cmd = 0x66;
void NordicBLE::sd_ble_version_get() {
	byte data[] = {0x01};
	this->send_command(sd_ble_version_get_cmd, data, 1);
}

// sd_ble_gap_adv_data_set
const uint8_t NordicBLE::sd_ble_gap_adv_data_set_cmd = 0x72;
void NordicBLE::sd_ble_gap_adv_data_set(byte *adv, uint8_t len) {
	byte data[len+4];
	uint8_t flg = 1;
	flg = 1;
	memcpy(&data[0], &len, 1);
	memcpy(&data[1], &flg, 1);
	memcpy(&data[2], adv, len);
	flg = 0;
	memcpy(&data[len+2], &flg, 1);
	memcpy(&data[len+3], &flg, 1);
	this->send_command(sd_ble_gap_adv_data_set_cmd, data, len+4);
}

// sd_ble_gap_device_name_set
const uint8_t NordicBLE::sd_ble_gap_device_name_set_cmd = 0x7C;
void NordicBLE::sd_ble_gap_device_name_set(const char *name) {
	uint16_t len = strlen(name);
	byte data[len+4];
	uint8_t flg = 0;
	memcpy(&data[0], &flg, 1);
	memcpy(&data[1], &len, 2);
	flg = 1;
	memcpy(&data[3], &flg, 1);
	memcpy(&data[4], name, len);
	this->send_command(sd_ble_gap_device_name_set_cmd, data, len+4);
}

// sd_ble_gap_adv_start
void NordicBLE::sd_ble_gap_adv_start() {
	byte data[] = {
		0x01,
		0x00, // GAP Advertising Type
		0x00, // Peer Address Present
		0x00, // Filter Policy
		0x00, // GAP Whitelist Present
		0x40, 0x06, // Interval(0.625 ms units)
		0x00, 0x00, // Timeout(No timeout)
		0x00 // Channel Mask
	};
	this->send_command(0x73, data, 10);
}

// sd_ble_gap_scan_start
void NordicBLE::sd_ble_gap_scan_start() {
	byte data[] = {
		0x01, 0x00, 0x00, 0xa0, 0x00, 0x50, 0x00, 0x00, 0x00
	};
	this->send_command(0x86, data, 9);
}

// sd_ble_gatts_service_add
const uint8_t NordicBLE::sd_ble_gatts_service_add_cmd = 0xA0;
void NordicBLE::sd_ble_gatts_service_add(uint8_t service_type, uint8_t uuid_type, uint16_t uuid) {
	byte data[] = {
		service_type, 0x01, 0x00, 0x00, uuid_type, 0x01
	};
	memcpy(&data[2], &uuid, 2);
	this->send_command(sd_ble_gatts_service_add_cmd, data, 6);
}

// sd_ble_gatts_characteristic_add
const uint8_t NordicBLE::sd_ble_gatts_characteristic_add_cmd = 0xA2;
void NordicBLE::sd_ble_gatts_characteristic_add(uint16_t service_handle, GattCharProps &char_props, uint8_t uuid_type, uint16_t uuid, byte *user_data, uint16_t data_len) {
	// TODO:
	byte data[] = {
		0x00, 0x00, // Service Handle
		0x01,
		0x00, // Characteristic Properties
		0x00, // Characteristic Extended Properties
		0x00, 0x00, // User Descriptor Maximum Size
		0x00, 0x00, // User Descriptor Size
		0x00, // User Descriptor Present
		0x00, // Presentation Format Structure Present
		0x00, // Attribute Metadata User Description Present
		0x00, // Attribute metadata for the Client Characteristic Configuration Present
		0x00, // Attribute metadata for the Server Characteristic Configuration Present
		0x01,
		0x01, // UUID Present
		0x00, 0x00, uuid_type, // Conditional: UUID
		0x01, // Attribute Metadata Present
		0x11, 0x11, 0x02,
		0x00, 0x00, // Value offset
		0x00, 0x00, // Maximum Value Length
		0x00, 0x00, // Value Length
		0x00, // Attribute Data Present
	};
	memcpy(&data[0], &service_handle, 2);
	memcpy(&data[3], &char_props, 1);
	memcpy(&data[16], &uuid, 2);
	int max_len = 31 + data_len;
	byte out_data[max_len];
	memcpy(&out_data[0], &data, 30);
	uint8_t flg = 0x01;
	if (user_data) {
		memcpy(&out_data[25], &data_len, 2);
		memcpy(&out_data[27], &data_len, 2);
		memcpy(&out_data[29], &flg, 1);
		memcpy(&out_data[30], user_data, data_len);
	}
	memcpy(&out_data[max_len-1], &flg, 1);
	this->send_command(sd_ble_gatts_characteristic_add_cmd, out_data, max_len);
}

 #pragma mark - Private

// Send command
void NordicBLE::send_command(byte cmd, byte *data, int size) {
	int len = size + 2;
	// Debug print
	#ifdef DEBUG_PRINT
	if (debug) {
		Debug.println(F("\n*Send command"));
		Debug.print(F("len:"));
		Debug.println(len | (len << 8) & 0xFF, DEC);
		Debug.print(F("cmd:"));
		Debug.println(cmd, HEX);
		Debug.print(F("dat:"));
		for (int i=0; i<size; i++) {
			Debug.print(data[i], HEX);
			if (i<size-1) Debug.print(F(","));
		}
		Debug.println(F(""));
	}
	#endif
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
