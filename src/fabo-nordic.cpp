#include "fabo-nordic.h"

#ifdef USE_HARDWARE_SERIAL
	FaboBLE::FaboBLE(HardwareSerial &serial) {
		ble = new NordicBLE(serial);
	}
#else
	FaboBLE::FaboBLE(SoftwareSerial &serial) {
		ble = new NordicBLE(serial);
	}
#endif

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
