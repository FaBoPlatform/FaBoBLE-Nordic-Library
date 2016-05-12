#ifndef NordicBLE_h
#define NordicBLE_h

#include <Arduino.h>
#include <SoftwareSerial.h>

#define NRF_BAUD_RATE 115200
#define Debug Serial
#define DEBUG_PRINT

class CommandParser;
class NordicEventHandler;

class NordicBLE
{
public:
	struct CommandData {
		int len;
		byte type;
		byte command;
		byte data[50];
	};
	struct GattCharProps {
		uint8_t broadcast       :1; /**< Broadcasting of the value permitted. */
		uint8_t read            :1; /**< Reading the value permitted. */
		uint8_t write_wo_resp   :1; /**< Writing the value with Write Command permitted. */
		uint8_t write           :1; /**< Writing the value with Write Request permitted. */
		uint8_t notify          :1; /**< Notications of the value permitted. */
		uint8_t indicate        :1; /**< Indications of the value permitted. */
		uint8_t auth_signed_wr  :1; /**< Writing the value with Signed Write Command permitted. */
	};
private:
	#ifdef USE_HARDWARE_SERIAL
		HardwareSerial *serial;
	#else
		SoftwareSerial *serial;
	#endif
	CommandParser *parser;
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
	void sd_ble_uuid_vs_add(uint8_t *uuid);
	void sd_ble_version_get();
	void sd_ble_gap_adv_data_set(byte *adv, uint8_t len);
	void sd_ble_gap_device_name_set(const char *name);
	void sd_ble_gap_adv_start();
	void sd_ble_gap_scan_start();
	void sd_ble_gatts_service_add(uint8_t service_type, uint8_t uuid_type, uint16_t uuid);
	void sd_ble_gatts_characteristic_add(uint16_t service_handle, GattCharProps &char_props, uint8_t uuid_type, uint16_t uuid, byte *user_data, uint16_t data_len);
	// event handler
	NordicEventHandler *handler;
	// command
	static const uint8_t sd_ble_enable_cmd;
	static const uint8_t sd_ble_uuid_vs_add_cmd;
	static const uint8_t sd_ble_version_get_cmd;
	static const uint8_t sd_ble_gap_adv_data_set_cmd;
	static const uint8_t sd_ble_gap_device_name_set_cmd;
	static const uint8_t sd_ble_gatts_service_add_cmd;
	static const uint8_t sd_ble_gatts_characteristic_add_cmd;
};

class NordicEventHandler
{
public:
	virtual void nrfReceive(NordicBLE::CommandData &data) {};
};

#endif
