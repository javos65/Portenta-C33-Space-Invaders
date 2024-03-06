/*****************************************************************************
* | File      	: Space Invaders Demo  / SpaceInvaders.ino
* | Function    :	Demo for Portenta C33  with HAT expansion board , SPI LCD with ILI9486 and BLE remote control interface
* | Info        : JV 2024
* | Github      : https://github.com/javos65/Portenta-C33-Space-Invaders
*----------------
* |	This version:   V1.0
* | Date        :   2024-03-16
* | IOriginal   :   Objecrt oriented setup : https://github.com/YXHYX/arduino-space-invaders
*
* Player Definitions
*
******************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

// Includes
#include <SPI.h>
#include <Adafruit_GFX.h>
//#include <TFT_ILI9163C.h>
#include <Waveshare_ILI9486.h>
#include "Control.h"
#include "Screen.h"

// Defines

#define BLACK     0x0000
#define BLUE      0x001F
#define RED       0xF800
#define GREEN     0x07E0
#define LIME	  0x2EFF
#define CYAN      0x07FF
#define DARK_CYAN 0x0DCB
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define WHITE     0xFFFF

namespace{}

class Player
{
private:
	int x,y;
	int prevx,prevy; //used to clear (better than fillScreen()!!)
	
	int buzzer; //for the buzzer
	
	int bulletx, bullety;
	int bullet2x, bullet2y;
	bool bulletShoot;
  bool bullet2Shoot;
	
	bool alive;
  bool explode;
	
	int score;
  Control* m_control;
	
    Waveshare_ILI9486 *tft; // TFT_ILI9163C *tft;
	int VRx, VRy, SW;

public:
	Player(Waveshare_ILI9486 *tft, int VRx, int VRy, int SW, int B);
	virtual ~Player();

	int getScore();
	
	int getBX();
	int getBY();
  int getBX2();
	int getBY2();
	bool collide(int x1, int y1);
	
	bool getAlive();
	
	void shoot();
	void stopShoot();
  void stopShoot2();
	void addPoints(int points = 10);
	
  void update();
  void render();
	void initControl();
	
};
#endif  PLAYER_H
