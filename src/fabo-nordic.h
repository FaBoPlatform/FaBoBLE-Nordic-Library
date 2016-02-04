#include "Arduino.h"
#include <SoftwareSerial.h>

class nordic
{
public:
  void setDebug(void);
  void init(void);
  bool setAdvParams(void);
  bool setMode(void);
  void setBeaconUuid(byte uuid[]);
  void setBeaconMajor(byte major[]);
  void setBeaconMinor(byte minor[]);
  bool sendBeacon(void);
  bool stopAdv(void);
  bool isRunning(void);
  void scan(byte data[]);
};

struct BeaconParam {
  byte uuid[16];
  byte major[2];
  byte minor[2];
};


extern nordic faboBLE;
