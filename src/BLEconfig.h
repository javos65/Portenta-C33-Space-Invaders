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
* BLE functions
*
******************************************************************************/

#ifndef BLECONF_H
#define BLECONF_H


#include <ArduinoBLE.h>

#define DKEY_RIGHTARROW	0xae
#define DKEY_LEFTARROW	0xac
#define DKEY_UPARROW		0xad
#define DKEY_DOWNARROW	0xaf
#define DKEY_FIRE		    0xa3
#define DKEY_ESCAPE		  0x1b // ascii escape
#define DKEY_ENTER		  0x0d // ascii carriage return
#define DKEY_TAB			  0x09 //ascii TAB
#define DKEY_PAUSE	    0xff


extern int BLEinit();
extern uint16_t BLEgetKey();
extern bool BLECheck();

#endif  BLECONF_H
