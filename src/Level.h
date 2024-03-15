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
* Level Configs
*
******************************************************************************/

#ifndef LEVEL_H
#define LEVEL_H

#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include "Control.h".
#include "Player.h"
#include "Enemy.h"


class Level
{
	
private:
	
	int m_difficulty;
	int B;
	int enemyAmount;
  int aTimer;
	
	bool levelCompleted;
	bool playerAlive;
	
	Player* m_player;
	Enemy* m_enemyArray[MAXENEMY*4];
  Control* C;
	Waveshare_ILI9486 *m_tft;
	
	//Getters
	int enemiesDead();
	
public:
	Level(int difficulty, Waveshare_ILI9486 *tft, Control *C,int B);
	virtual ~Level();
	
	//Getters
	int getScore();
  int getKills();
	bool getLevelCompleted();
	bool getPlayerAlive();
	int getDifficulty();
	//Setters
	void setDifficulty(int difficulty);

	//Initializers
	void init(int difficulty, Waveshare_ILI9486 *tft, Control *C, int B);
	void initPlayer(Waveshare_ILI9486 *tft, Control *C,int B);
	void initEnemies(int amount, Waveshare_ILI9486 *m_tft, int B, int Orbit);
	
	//functions
	void levelComplete();
	void update();
	void render();
	
};

#endif