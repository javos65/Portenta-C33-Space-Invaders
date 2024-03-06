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
* B Game definitions
*
******************************************************************************/

#ifndef GAME_H
#define GAME_H

// Includes
#include "Level.h"

// Defines
#define null 0

namespace{ }

class Game
{
public:


private:

	//Game variables
	int m_score;
	int m_levelScore;
	int m_highScore;
	bool m_gameOver;
	int m_difficulty;
	Level* m_level;
  uint32_t m_retimer;
	int VRx;
	int VRy;
	int SW;
	int B;
	//time variables
	long m_currentMillis;
	long m_previousMillis;

	Waveshare_ILI9486 *m_tft;
public:
    Game(Waveshare_ILI9486 *tft, int VRx, int VR, int SW, int B);
    virtual ~Game();
	
	
	void showScore(bool on);
	void levelComplete();
	void levelOver();
	
	void run(int interval);
    void render();
    void update();

	void init();

};

#endif // GAMEENGINE_H
