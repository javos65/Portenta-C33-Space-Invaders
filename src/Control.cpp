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
* BLE control object
*
******************************************************************************/
#include "Control.h"
#include "BLEconfig.h"

Control::Control()
  {
	this->pressed_x = 0, this->pressed_y = 0, this->pressed_s = 0;
	this->prevx = 0, this->prevy = 0, this->prevs = 0;
  this->keyvalue = 0;
  this->BLEconnected = 0;
}

Control::~Control(){  
}

void Control::getKeys(){
	     
       uint16_t c=0x0000;
       c = BLEgetKey();          // read ble scancode 16 bit 0xffxx = pressed, 0x00xx = depressed
          switch(c) {
              case 0xffae : {this->prevx = this->pressed_x;this->pressed_x = 1;break;} // right arrow pressed
              case 0xffac : {this->prevx = this->pressed_x;this->pressed_x = -1;break;} // left arrow pressed    
              case 0xffad : {this->prevy = this->pressed_y;this->pressed_y = 1;break;} // up arrow pressed
              case 0xffaf : {this->prevy = this->pressed_y;this->pressed_y = -1;break;} // dowb arrow pressed   
              case 0xffa3 : {this->prevs = this->pressed_s;this->pressed_s = 1;break;} // dowb arrow pressed      

              case 0x00ae : {this->prevx = this->pressed_x;this->pressed_x = 0;break;} // right arrow pressed
              case 0x00ac : {this->prevx = this->pressed_x;this->pressed_x = 0;break;} // left arrow pressed    
              case 0x00ad : {this->prevy = this->pressed_y;this->pressed_y = 0;break;} // up arrow pressed
              case 0x00af : {this->prevy = this->pressed_y;this->pressed_y = 0;break;} // dowb arrow pressed   
              case 0x00a3 : {this->prevs = this->pressed_s;this->pressed_s = 0;break;} // dowb arrow pressed         
              default : {;}
	            }
}

void Control::init(){	 
	this->pressed_x = 0, this->pressed_y = 0, this->pressed_s = 0;
	this->prevx = 0, this->prevy = 0, this->prevs = 0;
  this->keyvalue = 0;
}


int Control::getCX(){
	return this->pressed_x;
}

int Control::getCY(){
	return this->pressed_y;
}

int Control::getCS(){
	return this->pressed_s;
}

int Control::getBLE(){
  this->BLEconnected = BLECheck();
	return this->BLEconnected;
}
