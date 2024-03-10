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
//#include <Waveshare_ILI9486.h>
#include "Control.h"
#include "Level.h"
#include "Control.h"

// Defines
#define null 0

namespace{ }

class Game
{

private:

	//Game variables
	int m_score;
	int m_levelScore;
	int m_highScore;
  int m_highLevel;
  
	bool m_gameOver;
	int m_difficulty;
  int m_ships;
	Level* m_level;
  uint32_t m_retimer;
	int B;
	//time variables
	long m_currentMillis;
	long m_previousMillis;
  Control *m_C;
	Waveshare_ILI9486 *m_tft;
public:
    Game(Waveshare_ILI9486 *tft,  int B);
    virtual ~Game();
	
	
	void showScore(bool on);
	void levelComplete();
	void levelOver();
  void gameOver();
	
	void run(int interval);
  void render();
  void update();

  void newgame();
  void randomize();
	void initgraphics_control();
  void firstscreen();

};

#endif // GAMEENGINE_H
