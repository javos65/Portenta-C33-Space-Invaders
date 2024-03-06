
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
* Enemy Objects
*
******************************************************************************/
#include "Enemy.h"
#include "Screen.h"

//constructor 
Enemy::Enemy(Waveshare_ILI9486 *tft, int x, int y, int B,int Ti) : m_tft(tft) {
	m_x = x, m_y 	= y; 	//intialize position
  m_prevx=x, m_prevy=y;
	m_bulletx = 0, m_bullety = 0;
	animationFrame 	= 0; 	//intialize frame of animation
	alive 			= true;	//intialize life state
  explode     = 0;
	buzzer			= B;	//intialize the buzzer
	timerb 			= random(650, 730);
  timerx 			= 0;
  timery 			= 0;
  type        = Ti; // type of invader
  shift       = 0; // shift
  bombrate    = 1;
}
//deconstructor
Enemy::~Enemy(){
	delete m_tft; //yeet that pointer
}

//check collisions idk
bool Enemy::collide(int x1, int y1){
	if(x1 > m_x-1             &&
		 x1 < (m_x+ENEMYSX+1)   &&
		y1 > m_y-4              &&
		y1 <  (m_y+ENEMYSY+1)   ){
		//collision detected
		this->m_tft->drawLine(this->m_bulletx, this->m_bullety-8, this->m_bulletx, this->m_bullety+8, BLACK); // remove bullit
    this->shootBullet = false; // remove bomb indication
		tone(buzzer, 300, 130);
    this-> explode = 1;
    return true;
    //if (alive == true ) {this->explode = true; this->alive = false; return false;}
    //else if (explode == true) {this->alive = false;return true;}
	}
	return false;
}

void Enemy::shoot()
{
	this->timerb += bombrate;
	if( (this->timerb > 750) && (shootBullet == false) ){	//new bomb only when timer is up and no other bomb was falling
		this->m_bulletx = this->m_x + random(0, ENEMYSX-2);
		this->m_bullety = this->m_y+ENEMYSY-2;
		this->shootBullet = true;
		tone(buzzer, 1200, 50);
		this->timerb = random(0,  725);
	}
	
	if(this->shootBullet == true && this->alive == true) // if bullet was shooted, and enemy did not just die
	{
		this->m_bullety += 4;
		this->m_tft->drawLine(this->m_bulletx, this->m_bullety, this->m_bulletx, this->m_bullety-8, BLACK);
    //this->m_tft->writeFastVLine(this->m_bulletx, this->m_bullety-8, 8, BLACK);    

		if(this->m_bullety < BOMBEND-4)
			this->m_tft->drawLine(this->m_bulletx, this->m_bullety+8, this->m_bulletx, this->m_bullety, GREEN);
        // this->m_tft->writeFastVLine(this->m_bulletx, this->m_bullety, 8, GREEN);

		if(this->m_bullety > BOMBEND){
			this->m_bullety = this->m_y;
			this->shootBullet = false;
		}
	}
}

void Enemy::update(){
	
	if(animationFrame == 5)
		animationFrame = -1;
	else
		animationFrame++;
	shoot();

  this->timerx++; // timer for X-shift of enemy
  this->timery++; // timer for Y-shift of enemy
  if(this->timerx > ZIGZAGTICK){	
      this->timerx =0;
      this->m_prevx= this->m_x;;
      if(shift==1) {this->m_x-=ENEMYSX;;shift=0;}
      else {this->m_x+=ENEMYSX;;shift=1;}
      }
  else if( (this->timery > DOWNTICK ) && (m_y < BUNKERLIMIT) ){	// once bunker limit is reached, no more down
      this->timery =0;
      this->m_prevy= this->m_y;;
      this->m_y+=ENEMYSY;
      this->bombrate += 2; // increase bomb rate
      }      
}

void Enemy::render(){
  int i,j;

  if( (m_prevx != m_x)  || (m_prevy != m_y)){     // Clear old enemy, on old place if there was a change
      m_tft->fillRect(m_prevx, m_prevy,ENEMYSX,ENEMYSY, BLACK);
      this->m_prevx = m_x;this->m_prevy = m_y; // then update to new position
      }

if (explode) { 
      m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) Iexplode); 
      this->explode++; 
      if (this->explode >3) 
          {
            m_tft->fillRect(m_x, m_y,ENEMYSX,ENEMYSY, BLACK);
            this->alive = false; 
          }
  }
else {
			  if(animationFrame < 3 && animationFrame >= 0){
         switch(this->type) {
				    case 1 : m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) invader1_1); ;break;
				    case 2 : m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) invader2_1); ;break;
				    case 3 : m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) invader3_1); ;break;
            default : m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) invader1_1); ;break;   
           }               
        }
			  if(animationFrame < 6 && animationFrame >= 3){
          switch(this->type) {
				    case 1 : m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) invader1_2); ;break;
				    case 2 : m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) invader2_2); ;break;
				    case 3 : m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) invader3_2); ;break;
            default : m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) invader1_2); ;break;
            }  
			  }
     }
}

bool Enemy::getAlive(){
	return this->alive;
}

int Enemy::getX(){
	return this->m_x;
}

int Enemy::getY(){
	return this->m_y;
}

int Enemy::getBulletX(){
	return this->m_bulletx;
}

int Enemy::getBulletY(){
	return this->m_bullety;
}
