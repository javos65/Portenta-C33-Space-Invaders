
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
* Enemy deifnitions
*
******************************************************************************/

#ifndef ENEMY_H
#define ENEMY_H

// Includes
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include "Screen.h"


// Defines

namespace{ }

// Enemy class
class Enemy
{
private:
	int m_x, m_y; //position of enemy
	int m_prevx, m_prevy; //previous posiotion of enemy
	int m_bombx, m_bomby;
	bool dropBomb;
	int type;
	int timerb;
  int timerx;
  int timery;
	int buzzer; // for buzzer
	int shift;
  int bombrate;
  int sidewinder;
  int xdirection;
	bool alive;       // life state of enemy -> alive stops wafter  enemy till exploded and bombs ended
  bool twilight;    // twilight state of enemy, hit but last bomb falling, but cant be hit again
  bool exploded;    // exploding state 
  bool attack;
  int  explodingA;  // explode animation counter
	int animationFrame; //animation frame of enemy
	
	Waveshare_ILI9486 *m_tft; // a pointer to the display

public:
	// Constructor // Destructor
	Enemy(Waveshare_ILI9486 *m_tft, int x, int y, int B,int Ti);
	virtual ~Enemy();
	
	//Getters
	bool getAlive();
	int getX();
	int getY();
	int getBulletX();
	int getBulletY();
	int getType();
  bool getAttack();

 // Setters
  void setAttack();

 // Checkers
	bool collide(int x1, int y1);
		
	//functions
	void bomb(bool now, int speed);
	void update();
	void render();
};

#endif // !ENEMY_H