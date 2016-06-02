#include "fabo-nordic.h"

#ifdef USE_HARDWARE_SERIAL
	FaboBLE::FaboBLE(HardwareSerial &serial) {
		ble = new NordicBLE(serial);
		ble->handler = this;
		onScanned = NULL;
	}
#else
	FaboBLE::FaboBLE(SoftwareSerial &serial) {
		ble = new NordicBLE(serial);
		ble->handler = this;
		onScanned = NULL;
	}
#endif

void FaboBLE::nrfReceive(NordicBLE::CommandData &data) {
	// BLE_GAP_EVT_CONNECTED
	if (data.command == 0x10) {
		#ifdef DEBUG_PRINT
		if (ble->debug) {
			Debug.println(F("\n*BLE_GAP_EVT_CONNECTED"));
			Debug.print(F("Connection Handle:"));
			Debug.print(data.data[0], HEX);
			Debug.print(F(" "));
			Debug.println(data.data[1], HEX);
			Debug.print(F("Peer Address:"));
			for (int i=0; i<7; i++) {
				Debug.print(data.data[2+i], HEX);
				Debug.print(F(" "));
			}
			Debug.print(F("\nOwn Address:"));
			for (int i=0; i<7; i++) {
				Debug.print(data.data[9+i], HEX);
				Debug.print(F(" "));
			}
			Debug.print(F("\nIRK:"));
			Debug.println(data.data[17], HEX);
			Debug.print(F("GAP Connection Parameters:"));
			for (int i=0; i<8; i++) {
				Debug.print(data.data[18+i], HEX);
				Debug.print(F(" "));
			}
		}
		#endif
		if (onConnected) {
			uint16_t handle = data.data[0] | (data.data[1] << 8);
			onConnected(handle);
		}
	}
	// BLE_GAP_EVT_DISCONNECTED
	if (data.command == 0x11) {
		if (onDisconnected) {
			#ifdef DEBUG_PRINT
			if (ble->debug) {
				Debug.println(F("\n*BLE_GAP_EVT_CONNECTED"));
				Debug.print(F("Connection Handle:"));
				Debug.print(data.data[0], HEX);
				Debug.print(F(" "));
				Debug.println(data.data[1], HEX);
				Debug.print(F("Reason:"));
				Debug.println(data.data[2], HEX);
			}
			#endif
			uint16_t handle = data.data[0] | (data.data[1] << 8);
			uint8_t reason = data.data[2];
			onDisconnected(handle, reason);
		}
	}
	// BLE_GAP_EVT_ADV_REPORT
	if (data.command == 0x1b) {
		#ifdef DEBUG_PRINT
		if (ble->debug) {
			Debug.println(F("\n*BLE_GAP_EVT_ADV_REPORT"));
			Debug.print(F("Connection Handle:"));
			Debug.print(data.data[0], HEX);
			Debug.print(F(" "));
			Debug.println(data.data[1], HEX);
			Debug.print(F("Address Type:"));
			Debug.println(data.data[2], HEX);
			Debug.print(F("Address:"));
			for (int i=0; i<6; i++) {
				Debug.print(data.data[3+i], HEX);
			}
			Debug.print(F("\nRSSI:"));
			Debug.println((int8_t)data.data[9], DEC);
			Debug.print(F("Flags:"));
			Debug.println(data.data[10], HEX);
			Debug.print(F("DataLen:"));
			Debug.println(data.len, DEC);
			Debug.print(F("Data:"));
			for (int i=0; i<data.len-11; i++) {
				Debug.print(data.data[11+i], HEX);
			}
			Debug.print(F("\n"));
		}
		#endif
		// send event to handler
		ScanData scan = {0};
		scan.handle = data.data[0] | (data.data[1] << 8);
		scan.addressType = data.data[2];
		memcpy(&scan.address, &data.data[3], 8);
		scan.RSSI = data.data[9];
		scan.flags = data.data[10];
		scan.dataLen = data.len - 11;
		memcpy(&scan.data, &data.data[11], scan.dataLen);
		if (onScanned) {
			onScanned(scan);
		}
	}
	if (data.command == 0x50) {
		if (onWrite) {
			int len = data.data[21] | (data.data[22] << 8);
			byte wdata[len];
			memcpy(&wdata, &data.data[23], len);
			onWrite(wdata, len);
		}
	}
	if (data.command == NordicBLE::sd_ble_enable_cmd) {
		if (onReady) {
			onReady(ver, data.data[0]);
		}
	}
	if (data.command == NordicBLE::sd_ble_gap_adv_data_set_cmd) {
		if (onSetAdvData) {
			onSetAdvData(data.data[0]);
		}
	}
	if (data.command == NordicBLE::sd_ble_uuid_vs_add_cmd) {
		ble->sd_ble_gap_device_name_set(this->name);
	}
	if (data.command == NordicBLE::sd_ble_version_get_cmd) {
		if (onReady) {
			if (data.data[0] == 0) {
				ver.lmp = data.data[4];
				ver.companyID = data.data[5] | (data.data[6] << 8);
				ver.firmwareID = data.data[7] | (data.data[8] << 8);
			}
			ble->sd_ble_enable();
		}
	}
	if (data.command == NordicBLE::sd_ble_gap_device_name_set_cmd) {
		ble->sd_ble_version_get();
	}
	if (data.command == NordicBLE::sd_ble_gatts_service_add_cmd) {
		if (onServiceAdded) {
			uint16_t handle = data.data[4] | (data.data[5] << 8);
			onServiceAdded(handle, data.data[0]);
		}
	}
	if (data.command == NordicBLE::sd_ble_gatts_characteristic_add_cmd) {
		if (onCharacteristicAdded) {
			if (data.data[4] > 0) {
				uint16_t handle = data.data[5] | (data.data[6] << 8);
				onCharacteristicAdded(handle, data.data[0]);
			} else {
				onCharacteristicAdded(0, data.data[0]);
			}
		}
	}

}

void FaboBLE::init(void) {
	ble->sd_ble_version_get();
}

void FaboBLE::init(const char *name, uint8_t *uuid) {
	this->name = name;
	ble->sd_ble_uuid_vs_add(uuid);
}

void FaboBLE::scan(void) {
	ble->sd_ble_gap_scan_start();
}

void FaboBLE::tick(void) {
	ble->tick();
}

void FaboBLE::setDebug(bool flg) {
	ble->debug = flg;
}

void FaboBLE::startAdvertise(void) {
	ble->sd_ble_gap_adv_start();
}

void FaboBLE::setAdvData(byte *data, uint8_t dataLen) {
	ble->sd_ble_gap_adv_data_set(data, dataLen);
}

void FaboBLE::addService(uint16_t uuid, bool SIGUUID, bool primary) {
	uint8_t serviceType = primary ? 0x01 : 0x02;
	uint8_t uuidType = SIGUUID ? 0x01 : 0x02;
	ble->sd_ble_gatts_service_add(serviceType, uuidType, uuid);
}

void FaboBLE::addCharacteristic(uint8_t handle, byte *data, uint8_t dataLen, uint16_t uuid, bool SIGUUID) {
	// TODO:
	NordicBLE::GattCharProps props;
	props.read = 1;
	props.write = 1;
	uint8_t uuidType = SIGUUID ? 0x01 : 0x02;
	ble->sd_ble_gatts_characteristic_add(handle, props, uuidType, uuid, data, dataLen);
}
