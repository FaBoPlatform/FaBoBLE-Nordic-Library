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
	// BLE_GAP_EVT_ADV_REPORT
	if (data.command == 0x1b) {
		if (ble->debug) {
			Debug.println("\n*BLE_GAP_EVT_ADV_REPORT");
			Debug.write("Connection Handle:");
			Debug.print(data.data[1], HEX);
			Debug.println(data.data[2], HEX);
			Debug.write("Address Type:");
			Debug.println(data.data[3], HEX);
			Debug.write("Address:");
			for (int i=0; i<6; i++) {
				Debug.print(data.data[4+i], HEX);
			}
			Debug.write("\nRSSI:");
			Debug.println((int8_t)data.data[10], DEC);
			Debug.write("Flags:");
			Debug.println(data.data[11], HEX);
			Debug.write("DataLen:");
			Debug.println(data.len, DEC);
			Debug.write("Data:");
			for (int i=0; i<data.len-12; i++) {
				Debug.print(data.data[12+i], HEX);
			}
			Debug.write("\n");
		}
		// send event to handler
		ScanData scan = {0};
		scan.handle = data.data[1] | (data.data[2] << 8);
		scan.addressType = data.data[3];
		memcpy(&scan.address, &data.data[4], 8);
		scan.RSSI = data.data[10];
		scan.flags = data.data[11];
		scan.dataLen = data.len - 12;
		memcpy(&scan.data, &data.data[12], scan.dataLen);
		if (onScanned) {
			onScanned(scan);
		}
	}
	if (data.command == 0xA0) {
		if (onServiceAdded) {
			uint16_t handle = data.data[4] | (data.data[5] << 8);
			onServiceAdded(handle, data.data[0]);
		}
	}

}

void FaboBLE::init(void) {
	ble->sd_ble_enable();
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

void FaboBLE::setBaseUUID(uint8_t *uuid) {
	ble->sd_ble_uuid_vs_add(uuid);
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
	uint8_t uuidType = SIGUUID ? 0x01 : 0x02;
	ble->sd_ble_gatts_characteristic_add(handle, props, uuidType, uuid, data, dataLen);
}
