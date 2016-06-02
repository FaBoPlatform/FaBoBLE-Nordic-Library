#include <Arduino.h>
#include "utility/NordicBLE.h"

class FaboBLE : public NordicEventHandler
{
public:
	struct VersionInfo {
		int8_t lmp;
		uint16_t companyID;
		uint16_t firmwareID;
	};
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
	VersionInfo ver;
	const char *name;
public:
	void nrfReceive(NordicBLE::CommandData &data);
	#ifdef USE_HARDWARE_SERIAL
		FaboBLE(HardwareSerial &serial);
	#else
		FaboBLE(SoftwareSerial &serial);
	#endif
	void init(void);
	void init(const char *name, uint8_t *uuid);
	void scan(void);
	void tick(void);
	void setAdvData(byte *data, uint8_t dataLen);
	void startAdvertise(void);
	void addService(uint16_t uuid, bool SIGUUID = false, bool primary = true);
	void addCharacteristic(uint8_t handle, byte *data, uint8_t dataLen, uint16_t uuid, bool SIGUUID = false);
	void setDebug(bool flg = true);
	// event handler
	void (*onReady)(VersionInfo ver, int8_t error);
	void (*onConnected)(uint16_t handle);
	void (*onDisconnected)(uint16_t handle, uint8_t reason);
	void (*onScanned)(ScanData &data);
	void (*onSetAdvData)(int8_t error);
	void (*onServiceAdded)(uint16_t handle, uint8_t error);
	void (*onCharacteristicAdded)(uint16_t handle, uint8_t error);
	void (*onWrite)(byte *data, uint8_t len);
};
