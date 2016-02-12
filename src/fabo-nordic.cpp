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
