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
* Player object
* Initiates Plaey Laser
* Movement control of Player Laser
* Keeps track of shoot bullets
* Sense collision with Enemy Bombs
******************************************************************************/

#include "Player.h"
#include "Screen.h"


Player::Player(Waveshare_ILI9486 *tft, Control *C, int B) : tft(tft), C(C) {
	this->x = PLAYERROWX , this->y = PLAYERROWY;
	this->prevx = PLAYERROWX ,this->prevy=PLAYERROWY;
	this->bulletx = 0, this->bullety = 0;
  this->exbulletx = 0, this->exbullety = 0;
	this->bulletShoot = false;

  this->bullet2x = 0, this->bullet2y = 0;
  this->exbullet2x = 0, this->exbullet2y = 0;  
	this->bullet2Shoot = false;
	this->prevx = 0, this->prevy = 0;
	this->score = 0;
	this->buzzer = B;
	this->explode = false;
	this->alive = true;
  //tft->fillRect(x, y+10,PLAYERSX,PLAYERSY, YELLOW);
}

Player::~Player(){
    delete this->tft;
    delete this->C;
}

void Player::shoot(){
	
	if(this->C->getCS()==1){
		if(bulletShoot == false){
			this->bulletx = this->x+PLAYERSX/2;
			this->bullety = this->y;
      exbulletx = bulletx; exbullety=bullety;
			tone(this->buzzer, 1000, 100);
			this->bulletShoot = true;
		  }
		else if(bullet2Shoot == false && bullety < (GAMEY+SCREENSY/2) ){ // second shot ! - yeah - only when first shot is half way
			this->bullet2x = this->x+PLAYERSX/2;
			this->bullet2y = this->y;
      exbullet2x = bullet2x; exbullet2y=bullet2y;
			tone(this->buzzer, 1000, 100);
			this->bullet2Shoot = true;
		  }      
	  }

	if(bulletShoot == true){ // first bullet
    this->tft->fillRect(this->bulletx, this->bullety, RAYX, RAYY, BLACK); // emove old ray
		exbulletx = bulletx; exbullety=bullety; bullety-=8;		
    if(bullety >= SKYLIMIT)
            this->tft->drawColors((int16_t)bulletx,(int16_t)bullety,(int16_t) RAYX, (int16_t) RAYY,(uint16_t *) Ray);
		else
		{
			bulletShoot = false;
		}
	}

	if(bullet2Shoot == true){ // secondbullet
    this->tft->fillRect(this->bullet2x, this->bullet2y, RAYX, RAYY, BLACK); // emove old ray
		exbullet2x = bullet2x; exbullet2y=bullet2y; bullet2y -= 8;	
    if(bullet2y >= SKYLIMIT)
      this->tft->drawColors((int16_t)bullet2x,(int16_t)bullet2y,(int16_t) RAYX, (int16_t) RAYY,(uint16_t *) Ray);
		else
		{
			bullet2Shoot = false;
		}
	}

}



void Player::stopShoot()
{
  this->tft->fillRect(this->bulletx, this->bullety, RAYX, RAYY, BLACK); // emove old ray
  this->bulletx=this->x;this->bullety=this->y;
	bulletShoot = false;
}

void Player::stopShoot2()
{
  this->tft->fillRect(this->bullet2x, this->bullet2y, RAYX, RAYY, BLACK); // emove old ray
	this->bullet2x=this->x;this->bullet2y=this->y;
  bullet2Shoot = false;
}

int Player::getScore(){
	return this->score;
}

void Player::addPoints(int points){
	this->score = this->score + points;
}	

bool Player::collide(int x1, int y1){
	
	if( x1 < (x + PLAYERSX-1)  &&
		  x1 > x-1               &&
		  y1 < (y + PLAYERSY+2)  &&
		  y1 > (y +PLAYERSY/2)  )
	{
		tone(buzzer, 200, 150);
     this->x--;
    for(int i = 0; i<20; ++i)  // make & shake explosion :)
        {
        tft->fillRect(this->prevx, this->prevy,PLAYERSX,PLAYERSY, BLACK);
        this->prevx = x;this->prevy = y;       // then update to new position
        if(i%2) { x = x+3;   tft->drawColors((int16_t)this->x,(int16_t)this->y,(int16_t) PLAYERSX, (int16_t) PLAYERSY,(uint16_t *) laserexplode); }  // EXPLODE if HIT}   
        else    { x = x-3;   tft->drawColors((int16_t)this->x,(int16_t)this->y,(int16_t) PLAYERSX, (int16_t) PLAYERSY,(uint16_t *) laserexplode2);} // EXPLODE if HIT }  
        delay(50); // wait after collide
        }


    this->alive = false;
    return true;
	}
	
	return false;
}

void Player::update(){
  this->C->getKeys(); // update controller keys
  this->shoot();
	this->prevx = x;
	this->prevy = y;
	if(this->C->getCX()==1)
		x += 4;
	else if(this->C->getCX()==-1)
		x -= 4;
	if(this->C->getCY()==-1)
		y += 4;
	else if(this->C->getCY()==1)
		y -= 4;
	if(x > PLAYERENDX2)
		x = PLAYERENDX2;
	else if (x < PLAYERENDX1)
		x = PLAYERENDX1;
	
	if(y > PLAYERROWY)
		y = PLAYERROWY;
	else if (y < (PLAYERROWY-2*PLAYERSY))
		y = (PLAYERROWY-2*PLAYERSY);
	this->shoot();

}

void Player::render(){
int i,j;
 if (this->alive == true) {               // only render animation if alive, if not alive, explosion is already rendered
  if( (prevx != x)  || (prevy != y)){     // Clear old laser, on old place if there was a change
  	tft->fillRect(prevx, prevy,PLAYERSX,PLAYERSY, BLACK);
    this->prevx = x;this->prevy = y;       // then update to new position
    }
    tft->drawColors((int16_t)x,(int16_t)y,(int16_t) PLAYERSX, (int16_t) PLAYERSY,(uint16_t *) laser); 
 }
 
}

int Player::getBX(){
	return this->bulletx;
}

int Player::getBY(){
	return this->bullety;
}

int Player::getBX2(){
	return this->bullet2x;
}

int Player::getBY2(){
	return this->bullet2y;
}

bool Player::getAlive(){
	return this->alive;
}
