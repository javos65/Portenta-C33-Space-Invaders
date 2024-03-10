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
* Level Objects
* Initiates Level type
* Calls player Object, 
* Calls multiple Enemy objects, depending of Difficulty level
* Keeps track of enemy and player status
* Keeps track of object collisions
******************************************************************************/

#include "Level.h"
#include "Screen.h"
#include "Debug.h"

//Constructor / Destructor
Level::Level(int difficulty, Waveshare_ILI9486 *tft, Control *C, int B) : m_tft(m_tft), C(C) {
	this->init(difficulty, tft, C, B);
	this->levelCompleted = false;
	this->playerAlive = true;
}

Level::~Level()
{
	delete this->m_player;
	delete this->m_tft;
  delete this->C;
	for(int i = 0; i < enemyAmount; i++)
		delete this->m_enemyArray[i];
}

//Getters

int Level::enemiesDead()
{
  int d = 0;
	for(int i = 0; i < this->enemyAmount; i++)
		if(!this->m_enemyArray[i]->getAlive()){
				d++;
    }
	return d; // returning number of enemies dead
}

int Level::getScore()
{
	return this->m_player->getScore();
}

bool Level::getLevelCompleted()
{
	return this->levelCompleted;
}

bool Level::getPlayerAlive(){
	return this->playerAlive;
}

//Setters
void Level::setDifficulty(int difficulty)
{
	this->m_difficulty = difficulty;
}

//Initializers
void Level::init(int difficulty, Waveshare_ILI9486 *tft, Control *C, int B)
{
	this->setDifficulty(difficulty);
	this->initPlayer(tft, C, B);

	switch(this->m_difficulty)
	{
			//easy difficulty
		case 1:
			this->initEnemies(MAXENEMY*2, tft, B, 0);
			break;
			//normal difficulty
		case 2:
			this->initEnemies(MAXENEMY*3, tft, B,0);
			break;
			
			//hard difficulty
		case 3:
			this->initEnemies(MAXENEMY*4, tft, B,1);
			break;
			
			//harder difficulty
		case 4:
			this->initEnemies(MAXENEMY*3, tft, B,3);
			break;

 			//pro difficulty     
		case 5:
			this->initEnemies(MAXENEMY*4, tft, B,3);
			break;    

  			//nightmare difficulty           
		case 6:
			this->initEnemies(MAXENEMY*3, tft, B,5);
			break;  

   			//hell on earth difficulty               
		default:
    	this->initEnemies(MAXENEMY*4, tft, B,5);
			break;
	}
}


void Level::initPlayer(Waveshare_ILI9486 *tft, Control *C, int B)
{
	m_player = new Player(tft, C, B);
}

void Level::initEnemies(int amount, Waveshare_ILI9486 *tft, int B,int Orbit)
{
	this->enemyAmount = amount;
	for(int i = 0; i < this->enemyAmount; i++){
		if(i < (MAXENEMY) )
			this->m_enemyArray[i] = new Enemy(tft, ENEMYROWX+ i*ENEMYSX*2, ENEMYROWY + Orbit*ENEMYSY, B, 3);
		else if(i>=MAXENEMY && i<(MAXENEMY*2))
			this->m_enemyArray[i] = new Enemy(tft, ENEMYROWX+ (i-MAXENEMY)*ENEMYSX*2 +ENEMYSX/2, ENEMYROWY + 2*ENEMYSY + Orbit*ENEMYSY, B, 1);
		else if(i>=(2*MAXENEMY) && i < (3*MAXENEMY))
			this->m_enemyArray[i] = new Enemy(tft, ENEMYROWX+ (i-2*MAXENEMY)*ENEMYSX*2, ENEMYROWY + 4*ENEMYSY+ Orbit*ENEMYSY, B, 2);
		else if(i>=(3*MAXENEMY) && i < (4*MAXENEMY))
			this->m_enemyArray[i] = new Enemy(tft, ENEMYROWX+ (i-3*MAXENEMY)*ENEMYSX*2 +ENEMYSX/2, ENEMYROWY + 6*ENEMYSY+ Orbit*ENEMYSY, B, 1);		
    else if(i>=(4*MAXENEMY) && i < (5*MAXENEMY))
			this->m_enemyArray[i] = new Enemy(tft, ENEMYROWX+ (i-2*MAXENEMY)*ENEMYSX*2, ENEMYROWY + 4*ENEMYSY+ Orbit*ENEMYSY, B, 3);  
	}
}

//Functions

void Level::update()
{
	if(!this->levelCompleted || this->playerAlive)
	{
		this->m_player->update();
		for(int i = 0; i < this->enemyAmount; i++){
			//while the enemy is still alive
			if(this->m_enemyArray[i]->getAlive()){
				
				//check collisions of the player's bullet 
				if(this->m_enemyArray[i]->collide(this->m_player->getBX(), this->m_player->getBY())){
					this->m_player->stopShoot();					//stop the bullet
					this->m_player->addPoints(m_enemyArray[i]->getType());					//and add points to the player, different point per type
				}
				//check collisions of the player's bullet2 
				if(this->m_enemyArray[i]->collide(this->m_player->getBX2(), this->m_player->getBY2())){
					this->m_player->stopShoot2();					//stop the bullet2
					this->m_player->addPoints(1);					//and add points to the player
				}

				//if enemy's bomb collides with the player
				if(  this->m_player->collide(this->m_enemyArray[i]->getBulletX(), this->m_enemyArray[i]->getBulletY()) // colliding bomb
					|| this->m_player->collide(this->m_enemyArray[i]->getX(), this->m_enemyArray[i]->getY())  )          // colliding crash
					this->playerAlive = false;
				
				//update the enemy
				this->m_enemyArray[i]->update();
			}
		}
		if (this->enemiesDead()>=this->enemyAmount) this->levelCompleted =true;
	}
	
}

void Level::render()
{
	this->m_player->render();
	
	for(int i = 0; i < this->enemyAmount; i++){
		if(!this->m_enemyArray[i]->getAlive() )
			continue;
		this->m_enemyArray[i]->render();
	}
}