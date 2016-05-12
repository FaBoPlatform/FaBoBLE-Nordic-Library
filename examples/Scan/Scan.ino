//
// FaBo BLE Brick
//
// brick_serial_ble
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
  // Scan開始
  faboBLE.scan();
}

// Scan時のイベントハンドラ
void onScanned(FaboBLE::ScanData &data)
{
  Serial.println(F("\n*BLE_GAP_EVT_ADV_REPORT"));
  Serial.print(F("Connection Handle:"));
  Serial.println(data.handle, HEX);
  Serial.print(F("Address Type:"));
  Serial.println(data.addressType, HEX);
  Serial.print(F("Address:"));
  for (int i=0; i<6; i++) {
    Serial.print(data.address[i], HEX);
    Serial.print(F(" "));
  }
  Serial.print(F("\nRSSI:"));
  Serial.println(data.RSSI, DEC);
  Serial.print(F("Flags:"));
  Serial.println(data.flags, HEX);
  Serial.print(F("DataLen:"));
  Serial.println(data.dataLen, DEC);
  Serial.print(F("Data:"));
  for (int i=0; i<data.dataLen; i++) {
    Serial.print(data.data[i], HEX);
    Serial.print(F(" "));
  }
  Serial.print(F("\n"));
}

void setup()
{
  Serial.begin(9600);

  // Debugモードでログが詳細に表示されます
  //faboBLE.setDebug();
  // イベントハンドラ登録
  faboBLE.onReady = onReady;
  faboBLE.onScanned = onScanned;
  // 初期化
  faboBLE.init();
}

void loop()
{
  // BLE内部処理のためloop内で呼び出してください
  faboBLE.tick();
}
