/*****************************************************************************
* | File      	: Space Invaders Demo  / SpaceInvaders.ino
* | Function    :	Demo for Portenta C33  with HAT expansion board , SPI LCD with ILI9486 and BLE remote control interface
* | Info        : JV 2024
* | Github      : https://github.com/javos65/Portenta-C33-Space-Invaders
*----------------
* |	This version:   V2.0
* | Date        :   2024-03-16
* | IOriginal   :   Object oriented setup : https://github.com/YXHYX/arduino-space-invaders
*
* Game Functions :
* Run-function Loops forever
* Creates Intial Gaming playground 
* Instantiales Level Object
* Controls level status and Game-flow
* Keep track of (high) scores and space ships
******************************************************************************/

#include "Game.h"
#include "Screen.h"
#include "StoreData.h"
#include "Debug.h"

Game::Game(Waveshare_ILI9486 *tft, int B) : m_tft(tft), B(B) {	
	m_gameOver 		 = false;
	m_score 		 = 0;
  m_kills 		 = 0;
	m_currentMillis  = 0;
	m_previousMillis = 0;
  m_gameMillis = 0;
	m_levelScore 	 = 0;
  m_levelKills 	 = 0;
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
  firstscreen();                                                // Show Splash screen and build Game screen
  newgame();                                                    // Reset Game counters
  this->showScore(true);                                        // Show score counters
  m_gameMillis = millis();                                      // init game timer
  this->m_level = new Level(m_difficulty, m_tft,m_C, B);        // setup first level
	while(!m_gameOver){                                           // play one game
		m_currentMillis = millis();
    if(m_currentMillis - m_previousMillis >= interval){         // update the game if <interval> millisecond has passed
			  update();
			  render();
        m_previousMillis = m_currentMillis;
        }
    }  
  if (m_score > m_highScore) { handle_highscore(); }            // handle game over : check high score, save high score
  } // loop while(1)
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
  this->m_tft->drawLine(GAMEX, BOMBEND+2, GAMEX+SCREENSX, BOMBEND+2, ORANGE);
  for(int t =0 ; t<this->m_ships;t++) m_tft->drawColors((int16_t)LCDWIDTH-48+t*(MINIX+3),(int16_t)BOMBEND+11,(int16_t) MINIX, (int16_t) MINIY,(uint16_t *) shipcount); 
  if ( (BOMBEND-(m_levelKills+m_kills)) >0 ) m_tft->fillRect(GAMEX-6, BOMBEND-(m_levelKills+m_kills), 3, m_levelKills+m_kills, RED);
 
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
    this->m_levelScore=0;this->m_levelKills=0;
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
    this->m_levelScore=0;this->m_levelKills=0;
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
    this->m_levelScore=0;this->m_levelKills=0;
    this->m_retimer=0;
    showScore(true);
    m_gameOver = true; 
}


void Game::update(){
  // check BLE connectivity and set Icon
  if ( this->m_C->getBLE() ) m_tft->drawColors((int16_t) 60,(int16_t) 3*LOGOHEIGHT-15,(int16_t)ICONWIDTH,(int16_t) ICONHEIGHT, (uint16_t *)  BLE_on);
  else 	                     m_tft->drawColors((int16_t) 60,(int16_t) 3*LOGOHEIGHT-15,(int16_t)ICONWIDTH,(int16_t) ICONHEIGHT, (uint16_t *)  BLE_off);

	// update Level if game is not over
  if ( (!m_gameOver) && (this->m_level != null) ) {
      this->m_level->update();
      // keep track of score on the screen
      int sc = this->m_level->getScore();
      if( m_levelScore != sc ) {	
          m_levelKills = this->m_level->getKills();
  	      m_levelScore = sc;
          this->showScore(true);
          }
      } // if not game over
}

void Game::render(){
int i,t;
if( !m_gameOver && (this->m_level != null) )
  {
	this->m_level->render();


	if(!this->m_level->getPlayerAlive() )       // Player died   
	{
    if(m_retimer==0) {
      m_retimer = millis();  // start retimer
      this->m_ships--;
      showScore(true);
      }
		 if (m_levelScore>0) {                      // migrate score
        m_kills += m_levelKills; m_levelKills=0;  
        t=m_levelScore;
        for(i=0;i<t;++i){
          m_score++;delay(30);
          m_levelScore--;                       // update total score
          showScore(true);
          }
      }    
		if(this->m_ships <0 ) this->gameOver();      // if no more ships : game is over
    else this->levelOver();                      // next try
	}
	else if(this->m_level->getLevelCompleted() && this->m_level->getPlayerAlive() ) // Level completed 
	{
     if(m_retimer==0) m_retimer = millis();      // start retimer
		 if (m_levelScore>0) {                      // migrate score
        m_kills += m_levelKills; m_levelKills=0;
        t=m_levelScore;
        for(i=0;i<t;++i){
          m_score++;delay(30);
          m_levelScore--;                       // update total score
          showScore(true);
          }
      }
    this->levelComplete();
	}

  } // if( !this->m_gameOver )
}

void Game::handle_highscore(){
m_gameMillis  = millis()- m_gameMillis;                 // record game time
int min = (int) (m_gameMillis/(60*1000));
int sec = (int) ((m_gameMillis-(min*60*1000))/1000);
          this->m_highScore=m_score;
          this->m_highLevel=m_difficulty;
          this->m_highKills=m_kills;
          Write_Highscore(m_highScore, m_highLevel,m_gameMillis,m_highKills);
          m_tft->drawColors((int16_t) GAMEX+SCREENSX/2-SPAVEINVADERSX/2,(int16_t)(GAMEY+SCREENSY/2)-SPAVEINVADERSY,(int16_t)SPAVEINVADERSX,(int16_t) SPAVEINVADERSY, (uint16_t *)  SpaveInvaders);
          m_tft->setCursor(GAMEX + SCREENSX/2-80, (GAMEY+SCREENSY/2)+SPAVEINVADERSY/4 + 10);
          m_tft->setTextColor(CYAN); m_tft->setTextSize(2);
	        m_tft->print("HIGHSCORE ");m_tft->print(this->m_highScore);
          m_tft->setCursor(GAMEX + SCREENSX/2-100, (GAMEY+SCREENSY/2)+SPAVEINVADERSY/4 + 35);
          m_tft->print(this->m_highKills);m_tft->print(" kills in ");m_tft->print(min);m_tft->print(":");m_tft->print(sec );m_tft->print(" min.");
          delay(5000);
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
  m_highKills=8;
  Read_Highscore(&m_highScore, &m_highLevel, &m_gameMillis,&m_highKills); // read Hisghcores from file
  randomize();
}

void Game :: newgame(){
	this->m_gameOver = false;
	this->m_score = 0;	
  this->m_kills = 0;
	this->m_currentMillis  = 0;
	this->m_previousMillis = 0;
	this->m_levelScore 	 = 0;	
  this->m_levelKills = 0;
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
long tt=millis();
  m_tft->drawColors((int16_t) 0,(int16_t)0,(int16_t)LCDWIDTH,(int16_t) LCDHEIGHT,(uint16_t *) Splash);      // Splash Screen
   this->m_C->clearKeys();
  while ( millis()-tt < 4000)                                                               // Hidden secret : touch Splash Screen and QR code for App download appears
    {
    this->m_C->getKeys();
    if (this->m_C->getLINK()) {m_tft->drawColors((int16_t) 0,(int16_t)0,(int16_t)LCDWIDTH,(int16_t) LCDHEIGHT,(uint16_t *) SplashQR); delay(10000);break;}     // Splash Screen
    }

  m_tft->fillScreen(CLEAR);

  m_tft->drawColors((int16_t) 10,(int16_t)20,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT,(uint16_t *) RENlogo);
  m_tft->drawColors((int16_t) 10,(int16_t)LOGOHEIGHT+25,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT,(uint16_t *) Renesas2);
  m_tft->drawColors((int16_t) 10,(int16_t)3*LOGOHEIGHT+35,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT,(uint16_t *) Arduino);
  m_tft->drawColors((int16_t) 10,(int16_t)LCDHEIGHT-LOGOHEIGHT-10,(int16_t)LOGOWIDTH,(int16_t) LOGOHEIGHT, (uint16_t *)  EBVlogo);
  this->m_tft->drawLine(GAMEX-12, 20, GAMEX-12, LCDHEIGHT-30, CYAN);
  this->m_tft->drawLine(GAMEX-13, 20, GAMEX-13, LCDHEIGHT-30, CYAN-0x08);
  this->m_tft->setTextColor(ORANGE);  
	this->m_tft->setTextSize(1);
  m_tft->setCursor(GAMEX+30, 2);
  this->m_tft->print("HighScore at Level ");this->m_tft->print(m_highLevel);this->m_tft->print(" with ");this->m_tft->print(m_highScore);this->m_tft->print(" points");
}
