#include "fabo-nordic.h"

#ifdef USE_HARDWARE_SERIAL
	FaboBLE::FaboBLE(HardwareSerial &serial) {
		ble = new NordicBLE(serial);
		ble->on_event = &FaboBLE::onEventHandler;
	}
#else
	FaboBLE::FaboBLE(SoftwareSerial &serial) {
		ble = new NordicBLE(serial);
		ble->on_event = &FaboBLE::onEventHandler;
	}
#endif

void FaboBLE::onEventHandler(NordicBLE::CommandData &data) {
	// BLE_GAP_EVT_ADV_REPORT
	if (data.command == 0x1b) {
		// TODO: send this info to the user
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
		Debug.println(data.data[10], HEX);
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
