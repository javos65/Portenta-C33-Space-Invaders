/*****************************************************************************
* | File      	: Space Invaders Demo  / SpaceInvaders.ino
* | Function    :	Demo for Portenta C33  with HAT expansion board , SPI LCD with ILI9486 and BLE remote control interface
* | Info        : JV 2024
* | Github      : https://github.com/javos65/Portenta-C33-Space-Invaders
*----------------
* |	This version:   V1.0
* | Date        :   2024-03-16
* | Port        :   Arduino Portenta C33 BLE support
* | Info        :   BLE version for remote control via Smartphone App
* | IOriginal   :   Objecrtoriented setup : https://github.com/YXHYX/arduino-space-invaders
*
*  Portenta Carrier:     Portenta Hat Carrier  SKU ASX00049 Barcode 7630049204041
*  Portenta C33 board :  Portenta C33          SKU ABX00074 Barcode 7630049203556 
*
*
******************************************************************************/
#include "Game.h"
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include "Control.h"

//This Game Engine based on object c++

//Create a TFT object
Waveshare_ILI9486 tft; // change your setting for SPI interface in the library Waveshare_ILI9486.h  / .cpp

//Create Controller object
Control controller();

//Create a Game object
Game game(&tft, 6); // only Buzzer Pin, control using BLE routines poitnerm and pass tft screen poointer


void setup()
{
   if (!Serial) Serial.begin(115200);
   delay(2000); Serial.println("Serial initialized");
   Serial.println("BLE initialized"); 
   game.initgraphics_control(); 
   Serial.println("Graphics initialized");
   randomSeed(analogRead(A1));randomSeed(analogRead(A1));randomSeed(analogRead(A3)); //for random
}
void loop()
{
     game.run(50); //run the Game on an interval of 60ms;
}
