//
// FaBo BLE Brick
//
#include <SoftwareSerial.h>
#include <fabo-nordic.h>

SoftwareSerial serial(12, 13);
FaboBLE faboBLE(serial);

// 初期化完了イベント
void onReady(FaboBLE::VersionInfo ver, int8_t error)
{
  Serial.println(F("\n*onReady"));
  Serial.print(F("CompanyID: "));
  Serial.println(ver.companyID, HEX);
  Serial.print(F("FirmwareID: "));
  Serial.println(ver.firmwareID, HEX);
  Serial.print(F("LMPVersion: "));
  Serial.println(ver.lmp);
  Serial.print(F("Error: "));
  Serial.println(error, HEX);
  // Service追加
  faboBLE.addService(0x1234);
}

// Service追加イベント
void onServiceAdded(uint16_t handle, uint8_t error)
{
  Serial.println(F("\n*onServiceAdded"));
  Serial.print(F("Handle: "));
  Serial.println(handle, HEX);
  Serial.print(F("Error: "));
  Serial.println(error, HEX);
  // Characteristic追加
	byte data[] = {0x11, 0x22};
  faboBLE.addCharacteristic(handle, data, 2, 0x5678);
}

// Characteristic追加イベント
void onCharacteristicAdded(uint16_t handle, uint8_t error)
{
  Serial.println(F("\n*onCharacteristicAdded"));
  Serial.print(F("Handle: "));
  Serial.println(handle, HEX);
  Serial.print(F("Error: "));
  Serial.println(error, HEX);
  // アドバタイズ開始
  faboBLE.startAdvertise();
}

// 書き込みイベント
void onWrite(byte *data, uint8_t len)
{
  Debug.println(F("\n*onWrite"));
  Serial.print(F("Len: "));
  Debug.println(len, DEC);
  Serial.print(F("Data: "));
  for (int i=0; i<len; i++) {
    Debug.print(data[i], HEX);
    Debug.print(F(" "));
  }
  Debug.println(F(""));
}
// 接続された時に呼ばれる
void onConnected(uint16_t handle)
{
  Serial.println("\n*onConnected");
  Serial.print("handle:");
  Serial.println(handle, HEX);
}

// 接続が切れた時に呼ばれる
void onDisconnected(uint16_t handle, uint8_t reason)
{
  Serial.println("\n*onDisconnected");
  Serial.print("handle:");
  Serial.println(handle, HEX);
  Serial.print("reason:");
  Serial.println(reason, HEX);
  // アドバタイズ開始
  faboBLE.startAdvertise();
}

void setup()
{
  Serial.begin(9600);

  // Debugモードでログが詳細に表示されます
  faboBLE.setDebug();
  // イベントハンドラ登録
  faboBLE.onReady = onReady;
  faboBLE.onWrite = onWrite;
  faboBLE.onServiceAdded = onServiceAdded;
  faboBLE.onCharacteristicAdded = onCharacteristicAdded;
  faboBLE.onConnected = onConnected;
  faboBLE.onDisconnected = onDisconnected;
  // UUIDとデバイス名を指定して初期化
  uint8_t uuid[] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0a, 0x0b, 0x0c, 0x00, 0x00, 0x0f, 0x10,
  };
  faboBLE.init("myBLE", uuid);
}

void loop()
{
  // BLE内部処理のためloop内で呼び出してください
  faboBLE.tick();
}
