//
// FaBo BLE Brick
//
// brick_serial_ble
//
#include <SoftwareSerial.h>
#include <fabo-nordic.h>

SoftwareSerial serial(2, 3);
FaboBLE faboBLE(serial);
#define buttonPin A0 // ボタンピン

// ボタンの押下状況取得用
int buttonState = 0;
bool isFirst = false;

void setup()
{
  Serial.begin(9600);

  // ボタンピンを入力用に設定
  pinMode(buttonPin, INPUT);

  faboBLE.setDebug();
  faboBLE.init();
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
