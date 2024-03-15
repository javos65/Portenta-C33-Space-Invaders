/*****************************************************************************
* | File      	: Space Invaders Demo  / SpaceInvaders.ino
* | Function    :	Demo for Portenta C33  with HAT expansion board , SPI LCD with ILI9486 and BLE remote control interface
* | Info        : JV 2024
* | Github      : https://github.com/javos65/Portenta-C33-Space-Invaders
*----------------
* |	This version:   V2.0
* | Date        :   2024-03-16
* | IOriginal   :   Objecrt oriented setup : https://github.com/YXHYX/arduino-space-invaders
*
* Player Definitions
*
******************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

// Includes
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include "Control.h"
//#include "Screen.h"


namespace{}

class Player
{
private:
	int x,y;
	int prevx,prevy; //used to clear (better than fillScreen()!!)
	
	int buzzer; //for the buzzer
	
	int bulletx, bullety;
	int bullet2x, bullet2y;
	int exbulletx, exbullety;
	int exbullet2x, exbullet2y;  
	bool bulletShoot;
  bool bullet2Shoot;
	
	bool alive;
  bool explode;
	int ships;
	int score;
  int kills;
  Control* C;
	Waveshare_ILI9486 *tft; // TFT_ILI9163C *tft;

public:
	Player(Waveshare_ILI9486 *tft, Control *C, int B);
	virtual ~Player();

	int getScore();
  int getKills();
	
	int getBX();
	int getBY();
  int getBX2();
	int getBY2();
	bool collide(int x1, int y1);
	
	bool getAlive();
	
	void shoot();
	void stopShoot();
  void stopShoot2();
	void addPoints(int points);
  void addKill();
	
  void update();
  void render();
	
	
};
#endif  PLAYER_H
