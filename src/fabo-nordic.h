#include <Arduino.h>
#include "utility/NordicBLE.h"
#include <SoftwareSerial.h>

class FaboBLE : public NordicEventHandler
{
public:
	struct ScanData {
		uint16_t handle;
		uint8_t addressType;
		uint8_t address[8];
		int8_t RSSI;
		uint8_t flags;
		uint8_t dataLen;
		byte data[31];
	};
private:
	NordicBLE *ble;
public:
	void nrfReceive(NordicBLE::CommandData &data);
	#ifdef USE_HARDWARE_SERIAL
		FaboBLE(HardwareSerial &serial);
	#else
		FaboBLE(SoftwareSerial &serial);
	#endif
	void init(void);
	void scan(void);
	void tick(void);
	void setDebug(bool flg = true);
	// event handler
	void (*onScanned)(ScanData &data);
};
