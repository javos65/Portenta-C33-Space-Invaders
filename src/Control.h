/*****************************************************************************
* | File      	: Space Invaders Demo  / SpaceInvaders.ino
* | Function    :	Demo for Portenta C33  with HAT expansion board , SPI LCD with ILI9486 and BLE remote control interface
* | Info        : JV 2024
* | Github      : https://github.com/javos65/Portenta-C33-Space-Invaders
*----------------
* |	This version:   V1.0
* | Date        :   2024-03-16
* | IOriginal   :   Objecrtoriented setup : https://github.com/YXHYX/arduino-space-invaders
*
* Control Definitions
*
******************************************************************************/

#ifndef CONTROL_H
#define CONTROL_H

#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>

class Control
{
private:
	int pressed_x,pressed_y,pressed_s;
	int prevx,prevy,prevs; //used to clear (better than fillScreen()!!)
  char keyvalue;
  int BLEconnected,BLEactive ;
  Waveshare_ILI9486 *m_tft;
  TSPoint touch;
public:
	Control(Waveshare_ILI9486 *tft);
	virtual ~Control();

 void init();
 void getKeys();
 void clearKeys();
	int getCX();
	int getCY();
	int getCS();
  int getBLE();
	
};

#endif  CONTROL_H