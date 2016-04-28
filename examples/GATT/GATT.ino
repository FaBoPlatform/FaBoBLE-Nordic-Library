//
// FaBo BLE Brick
//
#include <SoftwareSerial.h>
#include <fabo-nordic.h>

SoftwareSerial serial(12, 13);
FaboBLE faboBLE(serial);

void onServiceAdded(uint16_t handle, uint8_t error)
{
  Serial.println("\n*onServiceAdded");
  Serial.print(handle, HEX);
  Serial.print(error, HEX);
	byte data[] = {0x11, 0x22};
  faboBLE.addCharacteristic(handle, data, 2, 0x5678);
}

void setup()
{
  Serial.begin(9600);

  faboBLE.setDebug();
  faboBLE.init();
  faboBLE.onServiceAdded = onServiceAdded;
  delay(300);
	uint8_t uuid[] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
	};
  faboBLE.setBaseUUID(uuid);
  delay(300);
  faboBLE.addService(0x1234);
  delay(300);
  faboBLE.startAdvertise();
}

void loop()
{
  // BLE内部処理のためloop内で呼び出してください
  faboBLE.tick();
}
