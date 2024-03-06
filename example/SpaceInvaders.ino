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
#include "BLEconfig.h" // BLE routines
//This Game Engine is supposed to help create games easily 

//Defines for TFT
//#define __CS 48
//#define __DC 

//Create a TFT object
	Waveshare_ILI9486 tft; // change your setting for SPI interface in the library Waveshare_ILI9486.h  / .cpp

//Create a Game object
Game game(&tft, A2, A3, 7, 6); // pins are not of interest, using BLE routines

void setup()
{
   if (!Serial) Serial.begin(115200);
   delay(2000); Serial.println("Serial initialized");
   if (BLEinit()) Serial.println("BLE initialized"); 
   
     //for random
     randomSeed(analogRead(A1));randomSeed(analogRead(A1));randomSeed(analogRead(A3));
     game.init();
}
void loop()
{
     game.run(60); //run the on an interval of 60ms;
}
