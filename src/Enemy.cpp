
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
* Controls automatic movement and animation
* Controls bombing 
* Sense Collision with laser bullets
******************************************************************************/
#include "Enemy.h"
#include "Screen.h"

//constructor 
Enemy::Enemy(Waveshare_ILI9486 *tft, int x, int y, int B,int Ti) : m_tft(tft) {
	m_x = x, m_y 	= y; 	//intialize position
  m_prevx=x, m_prevy=y;
	m_bombx = 0, m_bomby = 0;
	animationFrame 	= 0; 	//intialize frame of animation
	alive 			= true;	//intialize life state - callable by Level()
  twilight    = false; // twilight stage, when hit, but still exploding or falling bombs
  exploded    = false;
  explodingA    = 0;
	buzzer			= B;	//intialize the buzzer
	timerb 			= random(350, 820);
  timerx 			= 0;
  timery 			= 0;
  type        = Ti; // type of invader 1 / 2 / 3
  shift       = 0; // shift
  sidewinder  = 0 ;
  bombrate    = 1;
  dropBomb = false;
}
//deconstructor
Enemy::~Enemy(){
	delete m_tft; //yeet that pointer
}

//check collisions with laser
bool Enemy::collide(int x1, int y1){
  if(this->alive == true && this->twilight == false) { // only collide when alive, no collide in twilight
	if(x1 > m_x-1             &&
		 x1 < (m_x+ENEMYSX+1)   &&
		y1 > m_y-4              &&
		y1 <  (m_y+ENEMYSY+1)   ){
		//collision detected : Laser vs Enemy
		tone(buzzer, 300, 130);
    this-> explodingA = 1;     // mark start of explode animation
    this-> twilight = true; // mark enemy lives in twilight: no more new bombing, finish active bomb
    return true;
	  } 
  }

	return false;
}

void Enemy::bomb()
{
  if (this->alive == true && this->twilight == false) {    // start drop bombs only when alive and not in twilight
	  this->timerb += bombrate + this->type;
	  if( (this->timerb > 850) && (dropBomb == false) ){	//new bomb only when timer is up and no other bomb was falling
		  this->m_bombx = this->m_x + random(0, ENEMYSX-2);   ;
		  this->m_bomby = this->m_y+ENEMYSY-2;    
      switch(this->type) {    
        case 1 :  sidewinder = 0 ; break;
        case 2 :  sidewinder = random(-2,2 ); break; // side winder bombs : moving also sidewards
        case 3 :  sidewinder = random(-1,1); break; // side winder bombs : moving also sidewards
        default : sidewinder = 0; break;
      }
		  this->dropBomb = true;
		  tone(buzzer, 1200, 50);
		  this->timerb = random(0,  725);
	  }
  }	

	if(this->dropBomb == true ) // track fallen bomb
	{
    this->m_tft->fillRect(this->m_bombx, this->m_bomby, BOMBX, BOMBY, BLACK); // emove old Bomb
    this->m_bomby += BOMBSPEED; // update bomb positios, remember old
    this->m_bombx += sidewinder; // update bomb positios, remember old
    if (this->m_bombx < GAMEX+2 ||this->m_bombx > GAMEX+SCREENSX-2 ) sidewinder = -sidewinder;
		if(this->m_bomby < BOMBEND-BOMBSPEED){
        m_tft->drawColors((int16_t)m_bombx,(int16_t)m_bomby,(int16_t) BOMBX, (int16_t) BOMBY,(uint16_t *) Bomb);
    }
		else {
      m_bombx = 0, m_bomby = 0;
			this->dropBomb = false; 
		}
	}
}

void Enemy::update(){
	randomSeed(m_x*(analogRead(0)/m_y) ); // generate random seeds
	if(animationFrame == 5)	animationFrame = -1;
	else animationFrame++;
	
  if ( (this->twilight == true) && (this->dropBomb == false) && (this->exploded==true) ) this->alive = false; // mark finaly DEATH !! : no more rendering, no more collision, no more bombing
  else {
    bomb();
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
}

void Enemy::render(){
  int i,j;

  if( (m_prevx != m_x)  || (m_prevy != m_y)){     // Clear old enemy, on old place if there was a change
      m_tft->fillRect(m_prevx, m_prevy,ENEMYSX,ENEMYSY, BLACK);
      this->m_prevx = m_x;this->m_prevy = m_y; // then update to new position
      }

if (explodingA>0 && explodingA<4) { 
      m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) Iexplode); 
      this->explodingA++; 
      if (this->explodingA >= 4) {
        m_tft->fillRect(m_x, m_y,ENEMYSX,ENEMYSY, BLACK);
        this->exploded= true; // final explosion animation
      }
  }
else if(this->alive == true && this->twilight == false) { // only render when alive, but not in twilight
			  if(animationFrame < 3 && animationFrame >= 0){
         switch(this->type) {
				    case 1 : m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) invader1_1); ;break;
				    case 2 : m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) invader2_1); ;break;
				    case 3 : m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) invader3_1); ;break;
            default : m_tft->drawColors((int16_t)m_x,(int16_t)m_y,(int16_t) ENEMYSX, (int16_t) ENEMYSY,(uint16_t *) invader2_1); ;break;   
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

int Enemy::getType(){
	return this->type;
}

int Enemy::getX(){
	return this->m_x;
}

int Enemy::getY(){
	return this->m_y;
}

int Enemy::getBulletX(){
	return this->m_bombx;
}

int Enemy::getBulletY(){
	return this->m_bomby;
}
