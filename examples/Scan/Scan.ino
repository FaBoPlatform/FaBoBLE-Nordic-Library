//
// FaBo BLE Brick
//
// brick_serial_ble
//
#include <SoftwareSerial.h>
#include <fabo-nordic.h>

SoftwareSerial serial(12, 13);
FaboBLE faboBLE(serial);
#define buttonPin A0 // ボタンピン

// ボタンの押下状況取得用
int buttonState = 0;
bool isFirst = false;

// Scan時のイベントハンドラ
void onScanned(FaboBLE::ScanData &data)
{
  Serial.println("\n*BLE_GAP_EVT_ADV_REPORT");
  Serial.write("Connection Handle:");
  Serial.print(data.handle, HEX);
  Serial.write("\nAddress Type:");
  Serial.println(data.addressType, HEX);
  Serial.write("Address:");
  for (int i=0; i<6; i++) {
    Serial.print(data.address[i], HEX);
  }
  Serial.write("\nRSSI:");
  Serial.println(data.RSSI, DEC);
  Serial.write("Flags:");
  Serial.println(data.flags, HEX);
  Serial.write("DataLen:");
  Serial.println(data.dataLen, DEC);
  Serial.write("Data:");
  for (int i=0; i<data.dataLen; i++) {
    Serial.print(data.data[i], HEX);
  }
  Serial.write("\n");
}

void setup()
{
  Serial.begin(9600);

  // ボタンピンを入力用に設定
  pinMode(buttonPin, INPUT);

  //  faboBLE.setDebug();
  faboBLE.init();
  faboBLE.onScanned = onScanned;
  delay(100);
}

void loop()
{
  // BLE内部処理のためloop内で呼び出してください
  faboBLE.tick();
  // ボタンの押下状況を取得
  buttonState = digitalRead(buttonPin);

  // ボタン押下判定
  if (buttonState == HIGH && isFirst == false) {
    isFirst = true;
    faboBLE.scan();
    Serial.println("Button pressed.");
  } else {
    isFirst = false;
  }
}
