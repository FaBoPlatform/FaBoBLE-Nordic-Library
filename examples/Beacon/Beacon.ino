//
// FaBo BLE Brick
//
// brick_serial_ble
//
#include <SoftwareSerial.h>
#include <fabo-nordic.h>

SoftwareSerial serial(12, 13);
FaboBLE faboBLE(serial);

// Beaconデータ作成
void makeBeaconData(byte *uuid, int16_t major, int16_t minor, int8_t pow) {
  byte data[30] = {0};
  byte header[] = {0x02, 0x01, 0x06, 0x1A, 0xFF, 0x4C, 0x00, 0x02, 0x15};
  memcpy(&data[0], &header[0], 9);
  memcpy(&data[9], uuid, 16);
  int16_t swapped_major = (major>>8) | (major<<8);
  memcpy(&data[25], &swapped_major, 2);
  int16_t swapped_minor = (minor>>8) | (minor<<8);
  memcpy(&data[27], &swapped_minor, 2);
  memcpy(&data[29], &pow, 1);
  // Advデータ設定
  faboBLE.setAdvData(data, 30);
}

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
  // Beaconデータ作成
  byte uuid[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
  makeBeaconData(uuid, 100, 1000, 0xC9);
}

// Advデータ設定イベント
void onSetAdvData(int8_t error)
{
  Serial.println(F("\n*onSetAdvData"));
  Serial.print(F("Error: "));
  Serial.println(error, HEX);
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
  faboBLE.onSetAdvData = onSetAdvData;
  // 初期化
  faboBLE.init();
}

void loop()
{
  // BLE内部処理のためloop内で呼び出してください
  faboBLE.tick();
}
