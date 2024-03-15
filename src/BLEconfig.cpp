/*****************************************************************************
* | File      	: Space Invaders Demo  / SpaceInvaders.ino
* | Function    :	Demo for Portenta C33  with HAT expansion board , SPI LCD with ILI9486 and BLE remote control interface
* | Info        : JV 2024
* | Github      : https://github.com/javos65/Portenta-C33-Space-Invaders
*----------------
* |	This version:   V2.0
* | Date        :   2024-03-16
* | IOriginal   :   Objecrtoriented setup : https://github.com/YXHYX/arduino-space-invaders
*
* BLE configuration
*
******************************************************************************/
#include "BLEconfig.h"
#include "Arduino.h"

#define BLENAME "Invaders-Remote"
BLEService invadersService("19B10000-E8f2-537E-4f6C-B19B00B50001");             // Invaders wants B19-B00B5
BLEUnsignedIntCharacteristic invadersCharacteristic("19B10001-E8f2-537E-4f6C-B19B00B50001", BLERead | BLEWrite);
// Bluetooth® Low Energy Doom Remote Characteristic - custom 128-bit UUID, read and writable by 16 bit 

BLEDevice BLEcentral; 
bool BLE_Connected =false;

int BLEinit()
{
  pinMode(LEDG,OUTPUT);digitalWrite(LEDG, HIGH);
  pinMode(LEDB,OUTPUT);digitalWrite(LEDB, HIGH);
 if (BLE.begin()) {
      BLE.setLocalName(BLENAME);                                  // Set advertised local name and service UUID:
      BLE.setAdvertisedService(invadersService);
      invadersService.addCharacteristic(invadersCharacteristic);  // Add the characteristic to the service : 32bit read or write
      BLE.addService(invadersService);                            // Add service
      invadersCharacteristic.writeValue(0);                       // Set the initial value for the characeristic = 0, no key
      BLE.advertise();                                            // start advertising
      digitalWrite(LEDG, LOW);
      return 1;
      }
  else { return 0; }
}

uint16_t BLEgetKey(){
	uint16_t c=0;
    BLEcentral = BLE.central();
    if (BLEcentral) {
      digitalWrite(LEDB, LOW);digitalWrite(LEDG, HIGH);BLE_Connected =true;
      if ( invadersCharacteristic.written() ) {
          c = invadersCharacteristic.value();          // read ble scancode 16 bit 0xffxx = pressed, 0x00xx = depressed
          //Serial.print("["); Serial.print(c,HEX);Serial.println("]");
          }   
      }  
    else{
        digitalWrite(LEDB, HIGH);BLE_Connected =false;
        }
   
return c;
}


bool BLECheck()
{
  BLEcentral = BLE.central();
  if (BLEcentral) {BLE_Connected=true;digitalWrite(LEDB, LOW);}
  else  {BLE_Connected=false;digitalWrite(LEDB, HIGH);}
  return BLE_Connected;
}