#include "Arduino.h"

// #define USE_HARDWARE_SERIAL
#define NRF_BAUD_RATE 38400
#define Debug Serial

#ifndef USE_HARDWARE_SERIAL
  #include <SoftwareSerial.h>
#endif

class NordicBLE
{
private:
  #ifdef USE_HARDWARE_SERIAL
    HardwareSerial *serial;
  #else
    SoftwareSerial *serial;
  #endif
  bool debug = false;
  void sendCommand(byte type, byte *data, int size);
public:
  #ifdef USE_HARDWARE_SERIAL
    void init(HardwareSerial &serial, bool debug = false);
  #else
    void init(SoftwareSerial &serial, bool debug = false);
  #endif
  void sd_ble_enable(uint8_t service_changed = 0, uint32_t attr_tab_size = 0);
};
