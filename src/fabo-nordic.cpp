#include "fabo-nordic.h"
#include <SoftwareSerial.h>

BeaconParam beaconParam;

SoftwareSerial bleShield(12, 13);
bool DEBUG = false;
bool isRunningFlag = false;

void nordic::setDebug(){
    DEBUG = true;
}

void nordic::initnordic()
{
    // BLEとの通信用
    bleShield.begin(9600);
    
    if(DEBUG){
        Serial.begin(9600);
    }
}

void nordic::setBeaconUuid(byte uuid[]){
    
    memcpy(beaconParam.uuid, uuid, 16);
}

void nordic::setBeaconMajor(byte major[]){
    
    memcpy(beaconParam.major, major, 2);
}

void nordic::setBeaconMinor(byte minor[]){
    
    memcpy(beaconParam.minor, minor, 2);
}

bool nordic::sendBeacon(){
    
    byte command[6] = {0x00,  // message type  ->0x00:command
        0x20,     // Minimum payload length
        0x06,     // Message class -> 0x06:Generic Access Profile
        0x09,     // Message ID
        0x00,     // Advertisement data type -> 0x00: sets advertisement data e
        0x1e};    // Advertisement data to send
    
    byte beaconHeader[9] = {0x02,
        0x01,
        0x06,
        0x1A,
        0xFF,
        0x4C,
        0x00,
        0x02,
        0x15};
    byte txPower = 0xC9;
    
    for(int i = 0; i < 6; i++){
        bleShield.write((byte)command[i]);
    }
    for(int i = 0; i < 9; i++){
        bleShield.write((byte)beaconHeader[i]);
    }
    for(int i = 0; i < 16; i++){
        bleShield.write((byte)beaconParam.uuid[i]);
    }
    for(int i = 0; i < 2; i++){
        bleShield.write((byte)beaconParam.major[i]);
    }
    for(int i = 0; i < 2; i++){
        bleShield.write((byte)beaconParam.minor[i]);
    }
    
    bleShield.write((byte)0xC9);
    
    delay(1000);
    
    byte buffer[10];
    int i = 0;
    while (bleShield.available()) {
        buffer[i] = bleShield.read();
        i++;
    }
    
    if(buffer[0] == 0x00 && buffer[1] == 0x02 && buffer[2] == 0x06 && buffer[3] == 0x09){
        if(buffer[4] == 0x00 && buffer[5] == 0x00){
            isRunningFlag = true;
          
            return true;
        }
        else {
            if(DEBUG){
                Serial.println("(sendBeacon)Error Code");
                Serial.println(buffer[5],HEX);
                Serial.println(buffer[4],HEX);
            }
            return false;
        }
    } else {
        if(DEBUG){
            Serial.println("(sendBeacon)Unknow Error");
            Serial.println(buffer[0],HEX);
            Serial.println(buffer[1],HEX);
            Serial.println(buffer[2],HEX);
            Serial.println(buffer[3],HEX);
            Serial.println(buffer[4],HEX);
            Serial.println(buffer[5],HEX);
        }
        return false;
    }
}

bool nordic::setMode()
{
    
    byte command[6] = {0x00,  // message type  ->0x00:command
        0x02,     // Minimum payload length
        0x06,     // Message class -> 0x06:Generic Access Profile
        0x01,     // Message ID
        0x04,     // GAP Discoverable Mode
        0x00};    // GAP Connectable Mode
    
    for(int i = 0; i < 6; i++){
        bleShield.write((byte)command[i]);
    }
    delay(1000);
    
    byte buffer[10];
    int i = 0;
    while (bleShield.available()) {
        buffer[i] = bleShield.read();
        i++;
    }
    
    if(buffer[0] == 0x00 && buffer[1] == 0x02 && buffer[2] == 0x06 && buffer[3] == 0x01){
        if(buffer[4] == 0x00 && buffer[5] == 0x00){
            
            return true;
        }
        else {
            if(DEBUG){
                Serial.println("Error Code");
                Serial.println(buffer[5],HEX);
                Serial.println(buffer[4],HEX);
            }
            return false;
        }
    } else {
        if(DEBUG){
            Serial.println("Unknow Error");
            Serial.println(buffer[0],HEX);
            Serial.println(buffer[1],HEX);
            Serial.println(buffer[2],HEX);
            Serial.println(buffer[3],HEX);
            Serial.println(buffer[4],HEX);
            Serial.println(buffer[5],HEX);
        }
        return false;
    }
}


bool nordic::setAdvParams()
{
    byte command[9] = {0x00,
        0x05,
        0x06,
        0x08,
        0x00,
        0x02,
        0x00,
        0x02,
        0x07};
    
    for(int i = 0; i < 9; i++){
        bleShield.write((byte)command[i]);
    }
    delay(1000);
    
    byte buffer[10];
    int i = 0;
    while (bleShield.available()) {
        buffer[i] = bleShield.read();
        i++;
    }
    
    if(buffer[0] == 0x00 && buffer[1] == 0x02 && buffer[2] == 0x06 && buffer[3] == 0x08){
        if(buffer[4] == 0x00 && buffer[5] == 0x00){
            
            return true;
        }
        else {
            if(DEBUG){
                Serial.println("Error Code");
                Serial.println(buffer[5],HEX);
                Serial.println(buffer[4],HEX);
            }
            return false;
        }
    } else {
        if(DEBUG){
            Serial.println("Unknow Error");
            Serial.println(buffer[0],HEX);
            Serial.println(buffer[1],HEX);
            Serial.println(buffer[2],HEX);
            Serial.println(buffer[3],HEX);
            Serial.println(buffer[4],HEX);
            Serial.println(buffer[5],HEX);
        }
        return false;
    }
}


bool nordic::stopAdv()
{

  byte command[6] = {0x00,  // message type  ->0x00:command
                  0x02,     // Minimum payload length
                  0x06,     // Message class -> 0x06:Generic Access Profile
                  0x01,     // Message ID
                  0x00,     // GAP Discoverable Mode
                  0x00};    // GAP Connectable Mode

    for(int i = 0; i < 6; i++){ 
      bleShield.write((byte)command[i]);
    }
    delay(1000);

    byte buffer[10];
    int i = 0;
    while (bleShield.available()) {
      buffer[i] = bleShield.read();
      i++;
    }

    if(buffer[0] == 0x00 && buffer[1] == 0x02 && buffer[2] == 0x06 && buffer[3] == 0x01){
      if(buffer[4] == 0x00 && buffer[5] == 0x00){
        isRunningFlag = false;
        return true;
      }
      else {
        if(DEBUG){
          Serial.println("Error Code");
          Serial.println(buffer[5],HEX);
          Serial.println(buffer[4],HEX);
        }
        return false;
      }
    } else {
      if(DEBUG){
          Serial.println("Unknow Error");
          Serial.println(buffer[0],HEX);
          Serial.println(buffer[1],HEX);
          Serial.println(buffer[2],HEX);
          Serial.println(buffer[3],HEX);
          Serial.println(buffer[4],HEX);
          Serial.println(buffer[5],HEX);
        }
      return false;
    }
}

bool nordic::isRunning(){
    return isRunningFlag;
}

void nordic::scan(byte data[]){
    // ToDo
}

nordic faboBLE;
