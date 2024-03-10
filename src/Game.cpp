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
* Game Functions :
* Run-function Loops forever
* Creates Intial Gaming playground 
* Instantiales Level Object
* Controls level status and Game-flow
* Keep track of scores and space ships
******************************************************************************/

#include "Game.h"
#include "Screen.h"
#include "StoreData.h"
#include "Debug.h"

Game::Game(Waveshare_ILI9486 *tft, int B) : m_tft(tft), B(B) {	
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
	delete this->m_tft;
	delete this->m_level;
  delete this->m_C;
}


void Game::run(int interval){
while(1){
  firstscreen();                                                // Build Game Screen
  newgame();                                                    // clear Game settings
  this->m_level = new Level(m_difficulty, m_tft,m_C, B); // setup first level
	while(!m_gameOver){
		m_currentMillis = millis();
    // update the game if 100 millisecond has passed
    if(m_currentMillis - m_previousMillis >= interval){
			  update();
			  render();
        m_previousMillis = m_currentMillis;
        }
    }  
  if (m_score > m_highScore) {  // handle game over : check high score, save high score
          this->m_highScore=m_score;
          this->m_highLevel=m_difficulty;
          Write_Highscore(m_highScore, m_highLevel);
          m_tft->drawColors((int16_t) GAMEX+SCREENSX/2-SPAVEINVADERSX/2,(int16_t)(GAMEY+SCREENSY/2)-SPAVEINVADERSY,(int16_t)SPAVEINVADERSX,(int16_t) SPAVEINVADERSY, (uint16_t *)  SpaveInvaders);
          m_tft->setCursor(GAMEX + SCREENSX/2-80, (GAMEY+SCREENSY/2)+SPAVEINVADERSY/4 + 10);
          m_tft->setTextColor(CYAN); m_tft->setTextSize(2);
	        m_tft->print("HIGHSCORE ");m_tft->print(this->m_highScore);

          delay(5000);
	      }
  } // while(1)
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
  for(int t =0 ; t<this->m_ships;t++) m_tft->drawColors((int16_t)LCDWIDTH-48+t*(MINIX+3),(int16_t)BOMBEND+11,(int16_t) MINIX, (int16_t) MINIY,(uint16_t *) shipcount); 

}


void Game::levelComplete()
{
	
	m_tft->drawColors((int16_t) GAMEX+SCREENSX/2-LOGOWIDTH/2,(int16_t)(GAMEY+SCREENSY/2)-LOGOHEIGHT,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT, (uint16_t *)  LevelUp);
	this->m_tft->setTextColor(ORANGE); this->m_tft->setTextSize(2);
	this->m_tft->setCursor((GAMEX+SCREENSX/2)-80, (GAMEY+SCREENSY/2)+10);
	this->m_tft->println("Press joystick");
	this->m_tft->setCursor((GAMEX+SCREENSX/2)-90, (GAMEY+SCREENSY/2)+30);
	this->m_tft->println("button to continue");
  this->m_C->clearKeys();

	while ( this->m_C->getCS() != 1  && ( m_retimer!=0 && (millis()-m_retimer)<AUTORESTART )  ) {this->m_C->getKeys();;} // wait for key or timer

		m_difficulty++;
		m_level = null;
		this->m_tft->fillRect(GAMEX, GAMEY,LCDWIDTH-GAMEX-1,SCREENSY, BLACK); // clear players field
    showScore(true);
    this->m_levelScore=0;
    this->m_retimer=0;
    randomize();
		this->m_level = new Level(m_difficulty, m_tft, m_C, B);
    this->showScore(true);

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
	this->m_C->clearKeys();

	while ( this->m_C->getCS() != 1  && ( m_retimer!=0 && (millis()-m_retimer)<AUTORESTART )  ) {this->m_C->getKeys();} // wait for key or timer

		m_level = null;
		this->m_tft->fillRect(GAMEX, GAMEY,LCDWIDTH-GAMEX-1,SCREENSY, BLACK); // clear players field
		showScore(true);
    this->m_levelScore=0;
    this->m_retimer=0;
    randomize();
  	this->m_level = new Level(m_difficulty, m_tft,m_C, B);
    this->showScore(true);
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
  this->m_C->clearKeys();

	while ( this->m_C->getCS() != 1  && ( m_retimer!=0 && (millis()-m_retimer)<AUTORESTART )  ) {this->m_C->getKeys();} // wait for key or timer

		m_level = null;
		this->m_tft->fillRect(GAMEX, GAMEY,LCDWIDTH-GAMEX-1,SCREENSY, BLACK); // clear players field
		showScore(true);
    m_gameOver = true; 

}


void Game::update(){
  // check BLE connectivity and set Icon
  if ( this->m_C->getBLE() ) m_tft->drawColors((int16_t) 60,(int16_t) 3*LOGOHEIGHT-15,(int16_t)ICONWIDTH,(int16_t) ICONHEIGHT, (uint16_t *)  BLE_on);
  else 	                     m_tft->drawColors((int16_t) 60,(int16_t) 3*LOGOHEIGHT-15,(int16_t)ICONWIDTH,(int16_t) ICONHEIGHT, (uint16_t *)  BLE_off);

	// update Level if game is not over
  if (!m_gameOver) this->m_level->update();

  // keep track of score on the screen
  int sc = this->m_level->getScore();
  if(this->m_levelScore != sc) {	
  	this->m_levelScore = sc;
    this->showScore(true);
    }

}

void Game::render(){
int i;
if( !m_gameOver )
  {
	this->m_level->render();


	if(!this->m_level->getPlayerAlive() )       // Player died   
	{
    if(m_retimer==0) {
      m_retimer = millis();  // start retimer
      this->m_ships--;
      showScore(true);
      }
		if (this->m_levelScore>0) {              // migrate score, start timer
        for(i=0;i<m_levelScore;++i){
          this->m_score++;delay(50);
          this->m_levelScore--;                 // update total score
          showScore(true);
          }
      }    
		if(this->m_ships <0 ) this->gameOver();      // if no more ships : game is over
    else this->levelOver();                      // next try
	}
	else if(this->m_level->getLevelCompleted() && this->m_level->getPlayerAlive() ) // Level completed 
	{
     if(m_retimer==0) m_retimer = millis();   // start retimer
		 if (this->m_levelScore>0) {              // migrate score, start timer
        for(i=0;i<m_levelScore;++i){
          this->m_score ++;delay(50);
          this->m_levelScore--;                 // update total score
          showScore(true);
          }
      }
    this->levelComplete();
	}

  } // if( !this->m_gameOver )
}

void Game::initgraphics_control(){
  m_C = new Control(m_tft);
  m_C->init();
	m_tft->begin();
	m_tft->fillScreen(CLEAR);
	for(int i = 1; i < 4; i++)
	{
		m_tft->setRotation(i);
		m_tft->fillScreen(CLEAR);
	}
	m_tft->setRotation(3); // Landscape
  // load high scores

  m_highScore=10;
  m_highLevel=1;
  Read_Highscore(&m_highScore, &m_highLevel); // read Hisghcores from file
  randomize();
}

void Game :: newgame(){
	this->m_gameOver = false;
	this->m_score = 0;
	this->m_currentMillis  = 0;
	this->m_previousMillis = 0;
	this->m_levelScore 	 = 0;
	this->m_difficulty 	 = 1;
  this->m_retimer = 0;
  this->m_ships = 3;
  randomize();
}

void Game :: randomize()
{
  long t=millis(); for(int i = 0 ; i< (t&0x00ff);++i )  randomSeed((analogRead(0)) ); // generate random seed
}

void Game::firstscreen(){

  m_tft->drawColors((int16_t) 0,(int16_t)0,(int16_t)LCDWIDTH,(int16_t) LCDHEIGHT,(uint16_t *) Splash);      // Splash Screen
  delay(3000);
  m_tft->fillScreen(CLEAR);

  m_tft->drawColors((int16_t) 10,(int16_t)20,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT,(uint16_t *) RENlogo);
  m_tft->drawColors((int16_t) 10,(int16_t)LOGOHEIGHT+25,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT,(uint16_t *) Renesas2);
  m_tft->drawColors((int16_t) 10,(int16_t)3*LOGOHEIGHT+35,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT,(uint16_t *) Arduino);
  m_tft->drawColors((int16_t) 10,(int16_t)LCDHEIGHT-LOGOHEIGHT-10,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT, (uint16_t *)  EBVlogo);
	showScore(true);
  this->m_tft->drawLine(GAMEX-12, 20, GAMEX-12, LCDHEIGHT-30, CYAN);
  this->m_tft->drawLine(GAMEX-13, 20, GAMEX-13, LCDHEIGHT-30, CYAN-0x08);
  this->m_tft->setTextColor(ORANGE);  
	this->m_tft->setTextSize(1);
  m_tft->setCursor(GAMEX+30, 2);
  this->m_tft->print("HighScore at Level ");this->m_tft->print(m_highLevel);this->m_tft->print(" with ");this->m_tft->print(m_highScore);this->m_tft->print(" points");
}
