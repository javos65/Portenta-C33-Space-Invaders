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
* Game Functions & objects
*
******************************************************************************/

#include "Game.h"
#include "Screen.h"
#include "BLEconfig.h"

Game::Game(Waveshare_ILI9486 *tft, int B) 
	: m_tft(tft), B(B)
{	
	m_gameOver 		 = false;
	m_score 		 = 0;
	m_currentMillis  = 0;
	m_previousMillis = 0;
	m_levelScore 	 = 0;
	m_difficulty 	 = 1;
  m_retimer = 0;
  m_ships = 3; // number of spare ships for one game

}
Game::~Game(){
	delete m_tft;
	delete m_level;
}

void Game::showScore(bool on){
	if(!on)
	{
		m_tft->fillRect(GAMEX, BOMBEND+4, SCREENSX, 30, BLACK);
		return;
	}
	
	//if ( !this->m_level->getLevelCompleted() || !this->m_level->getPlayerAlive()) this->m_levelScore = this->m_level->getScore();
	
	m_tft->fillRect(GAMEX, BOMBEND+4, LCDWIDTH-GAMEX, 30, BLACK);
	m_tft->setCursor(GAMEX-8, BOMBEND+10);
	m_tft->setTextColor(WHITE); m_tft->setTextSize(2);
	m_tft->print("Level#");m_tft->print(this->m_difficulty);  m_tft->print(":");m_tft->print(this->m_levelScore);
  m_tft->setCursor(GAMEX + SCREENSX/2-12, BOMBEND+10);
  m_tft->print("Score:");	m_tft->print(this->m_score);
	this->m_tft->drawLine(GAMEX, BOMBEND, GAMEX+SCREENSX, BOMBEND, CYAN);
  this->m_tft->drawLine(GAMEX, BOMBEND+1, GAMEX+SCREENSX, BOMBEND+1, ORANGE);
  this->m_tft->drawLine(GAMEX, BOMBEND+2, GAMEX+SCREENSX, BOMBEND+2, CYAN);
  for(int t =0 ; t<this->m_ships;t++) m_tft->drawColors((int16_t)LCDWIDTH-56+t*(MINIX+3),(int16_t)BOMBEND+11,(int16_t) MINIX, (int16_t) MINIY,(uint16_t *) shipcount); 

}

void Game::run(int interval){
while(1){
  firstscreen();
  this->m_level = new Level(m_difficulty, m_tft, B, m_ships);
	while(!m_gameOver){
		m_currentMillis = millis();
        // update the game if 100 millisecond has passed
        if(m_currentMillis - m_previousMillis >= interval){
			    update();
			    render();
          m_previousMillis = m_currentMillis;
        }
	}
   //  handle game over : check high score, save high score
 } 
}

void Game::levelComplete()
{
	if(m_difficulty > 4)
		m_difficulty = 0;
	
	m_tft->drawColors((int16_t) GAMEX+SCREENSX/2-LOGOWIDTH/2,(int16_t)(GAMEY+SCREENSY/2)-LOGOHEIGHT,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT, (uint16_t *)  LevelUp);
	this->m_tft->setTextColor(ORANGE); this->m_tft->setTextSize(2);
	this->m_tft->setCursor((GAMEX+SCREENSX/2)-80, (GAMEY+SCREENSY/2)+10);
	this->m_tft->println("Press joystick");
	this->m_tft->setCursor((GAMEX+SCREENSX/2)-90, (GAMEY+SCREENSY/2)+30);
	this->m_tft->println("button to continue");
	
	if(BLEgetKey()==(0xff00 | DKEY_FIRE) || ( m_retimer!=0 && (millis()-m_retimer)>AUTORESTART )  )
	{
		m_difficulty++;
		m_level = null;
		this->m_tft->fillRect(GAMEX, 0,LCDWIDTH-GAMEX,LCDHEIGHT, BLACK); // clear players field
    showScore(true);
    this->m_levelScore=0;
    this->m_retimer=0;
		this->m_level = new Level(m_difficulty, m_tft, B, this->m_ships);
    this->showScore(true);
	}
}

void Game::levelOver()
{
	m_tft->drawColors((int16_t) GAMEX+SCREENSX/2-LOGOWIDTH/2,(int16_t)(GAMEY+SCREENSY/2)-LOGOHEIGHT,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT, (uint16_t *)  LevelOver);
	this->m_tft->setTextColor(YELLOW);  
	this->m_tft->setTextSize(2);
	this->m_tft->setCursor( (GAMEX+SCREENSX/2)-80, (GAMEY+SCREENSY/2)+10);
	this->m_tft->println("Press joystick");
	this->m_tft->setCursor( (GAMEX+SCREENSX/2)-102, (GAMEY+SCREENSY/2)+30);
	this->m_tft->print("button to try again");
	
	if( BLEgetKey() == (0xff00 | DKEY_FIRE) || ( m_retimer!=0 && (millis()-m_retimer)>AUTORESTART ) )
	{
		m_level = null;
		this->m_tft->fillRect(GAMEX, 0,LCDWIDTH-GAMEX,LCDHEIGHT, BLACK); // clear players field
		showScore(true);
    this->m_levelScore=0;
    this->m_retimer=0;
  	this->m_level = new Level(m_difficulty, m_tft, B,this->m_ships);
    this->showScore(true);
	}
}

void Game::gameOver()
{
	m_tft->drawColors((int16_t) GAMEX+SCREENSX/2-LOGOWIDTH/2,(int16_t)(GAMEY+SCREENSY/2)-LOGOHEIGHT,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT, (uint16_t *)  GameOver);
	this->m_tft->setTextColor(YELLOW);  
	this->m_tft->setTextSize(2);
	this->m_tft->setCursor( (GAMEX+SCREENSX/2)-80, (GAMEY+SCREENSY/2)+10);
	this->m_tft->println("Press joystick");
	this->m_tft->setCursor( (GAMEX+SCREENSX/2)-80, (GAMEY+SCREENSY/2)+30);
	this->m_tft->println("for new game");
	
	if( BLEgetKey() == (0xff00 | DKEY_FIRE) || ( m_retimer!=0 && (millis()-m_retimer)>AUTORESTART ) )
	{
		m_level = null;
		this->m_tft->fillRect(GAMEX, 0,LCDWIDTH-GAMEX,LCDHEIGHT, BLACK); // clear players field
		showScore(true);
    m_gameOver = true; 
	}
}
void Game::update(){
  // check BLE connectivity and set Icon
  if ( BLECheck() ) m_tft->drawColors((int16_t) 60,(int16_t) 3*LOGOHEIGHT-15,(int16_t)ICONWIDTH,(int16_t) ICONHEIGHT, (uint16_t *)  BLE_on);
  else 	            m_tft->drawColors((int16_t) 60,(int16_t) 3*LOGOHEIGHT-15,(int16_t)ICONWIDTH,(int16_t) ICONHEIGHT, (uint16_t *)  BLE_off);

	// update Level
  this->m_level->update();

  int sc = this->m_level->getScore();
  if(this->m_levelScore != sc) {	
  	this->m_levelScore = sc;
    this->showScore(true);
    }

}

void Game::render(){

if( !m_gameOver )
  {
	this->m_level->render();

	while(!this->m_level->getPlayerAlive() ) //|| this->m_level == null
	{
    if(m_retimer==0) {
      m_retimer = millis();  // start retimer
      this->m_ships--;
      showScore(true);
      }
    if (this->m_levelScore>0) {             // migrate level score to score
        this->m_score ++;delay(50);
        this->m_levelScore--; // update total score
        showScore(true);
      }
		if(this->m_ships <0 ) this->gameOver();      // if no more ships : game is over
    else this->levelOver();                      // next try
    if (m_gameOver) break;                       // get out of the loop when game is over
	}
	
	while(this->m_level->getLevelCompleted() )
	{
     if(m_retimer==0) m_retimer = millis();   // start retimer
		 if (this->m_levelScore>0) {              // migrate score, start timer
        this->m_score ++;delay(50);
        this->m_levelScore--;                 // update total score
        showScore(true);
        }
    this->levelComplete();
	}

  } // if( !this->m_gameOver )
}

void Game::init(){

	m_tft->begin();
	m_tft->fillScreen(CLEAR);
	
	for(int i = 1; i < 4; i++)
	{
		m_tft->setRotation(i);
		m_tft->fillScreen(CLEAR);
	}
	m_tft->setRotation(3);

}

void Game::firstscreen(){

	m_gameOver 	 = false;
	m_score 		 = 0;
	m_currentMillis  = 0;
	m_previousMillis = 0;
	m_levelScore 	 = 0;
	m_difficulty 	 = 1;
  m_retimer = 0;
  m_ships = 3;

  m_tft->drawColors((int16_t) 0,(int16_t)0,(int16_t)LCDWIDTH,(int16_t) LCDHEIGHT,(uint16_t *) Splash); // Splash Screen
  delay(3000);
  m_tft->fillScreen(CLEAR);

    m_tft->drawColors((int16_t) 10,(int16_t)20,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT,(uint16_t *) RENlogo);
    m_tft->drawColors((int16_t) 10,(int16_t)LOGOHEIGHT+25,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT,(uint16_t *) Renesas2);
    m_tft->drawColors((int16_t) 10,(int16_t)3*LOGOHEIGHT+35,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT,(uint16_t *) Arduino);
    m_tft->drawColors((int16_t) 10,(int16_t)LCDHEIGHT-LOGOHEIGHT-10,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT, (uint16_t *)  EBVlogo);
	showScore(true);
  this->m_tft->drawLine(GAMEX-12, 20, GAMEX-12, LCDHEIGHT-30, CYAN);
  this->m_tft->drawLine(GAMEX-13, 20, GAMEX-13, LCDHEIGHT-30, CYAN-0x08);
}
