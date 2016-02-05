#include <Arduino.h>
#include "utility/NordicBLE.h"
#include <SoftwareSerial.h>

class FaboBLE
{
private:
	NordicBLE *ble;
public:
	#ifdef USE_HARDWARE_SERIAL
		FaboBLE(HardwareSerial &serial);
	#else
		FaboBLE(SoftwareSerial &serial);
	#endif
	void init(void);
	void scan(void);
	void tick(void);
	void setDebug(bool flg = true);
};
