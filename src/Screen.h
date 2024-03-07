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
* Screen definitions
*
******************************************************************************/
#ifndef SCREEN_H
#define SCREEN_H
#include <Arduino.h>


//
// SCREEN and GRAPHICS definitions for Game
//
//
//


#define CLEAR     0x0005
#define BLACK     0x0000
#define BLUE      0x001F
#define RED       0xF800
#define GREEN     0x07E0
#define LIME	    0x2EFF
#define CYAN      0x07FF
#define DARK_CYAN 0x0DCB
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define WHITE     0xFFFF
#define ORANGE    0xfd20

#define LCDWIDTH 480  // portrait mode
#define LCDHEIGHT 320

#define AUTORESTART 20000

#define GAMESCALE 1
#define GAMEX 190 // absolute start game screen
#define GAMEY 30  // absolute start geme screen
#define SCREENSX 270
#define SCREENSY 260
#define LOGOWIDTH 150
#define LOGOHEIGHT 55

// enemy 
#define ENEMYSX  22
#define ENEMYSY   14
#define ENEMYROWX (GAMEX )       // leave one 2* size on the side
#define ENEMYROWY (GAMEY)         // leave one 2* size on the side
#define BOMBEND    (GAMEY + SCREENSY)         // the bottom
#define MAXENEMY   ( SCREENSX/(GAMESCALE*ENEMYSX*2))  // Max Enemy per row
#define BUNKERLIMIT (BOMBEND - ENEMYSY*4)      // the dow limit for invaders

#define ZIGZAGTICK  30    // time tick between zig zag
#define DOWNTICK    250   // time tich between lower orbit

// Player
#define SKYLIMIT GAMEY              // the SKY
#define PLAYERSX    24
#define PLAYERSY    24
#define PLAYERROWX  (GAMEX+ SCREENSX/2)
#define PLAYERROWY  BOMBEND-PLAYERSY 
#define PLAYERENDX1 (GAMEX)
#define PLAYERENDX2 (GAMEX+SCREENSX-PLAYERSX)
 
#define ICONHEIGHT  55
#define ICONWIDTH   55

#define MINIX   12
#define MINIY   10

extern  const uint16_t laserexplode2[PLAYERSY][PLAYERSX];
extern  const uint16_t laserexplode[PLAYERSY][PLAYERSX];
extern  const uint16_t laser[PLAYERSY][PLAYERSX] ;
extern  const uint16_t LevelUp[LOGOHEIGHT][LOGOWIDTH];
extern  const uint16_t GameOver[LOGOHEIGHT][LOGOWIDTH];
extern const int16_t LevelOver[LOGOHEIGHT][LOGOWIDTH]  ;
extern  const uint16_t EBVlogo[LOGOHEIGHT][LOGOWIDTH];
extern  const uint16_t RENlogo[LOGOHEIGHT][LOGOWIDTH];
extern  const uint16_t invader3_2[ENEMYSY][ENEMYSX];
extern  const uint16_t invader3_1[ENEMYSY][ENEMYSX];
extern  const uint16_t invader2_2[ENEMYSY][ENEMYSX];
extern  const uint16_t invader2_1[ENEMYSY][ENEMYSX];
extern  const uint16_t invader1_2[ENEMYSY][ENEMYSX];
extern  const uint16_t invader1_1[ENEMYSY][ENEMYSX];
extern  const uint16_t Iexplode[ENEMYSY][ENEMYSX];
extern const uint16_t Arduino[LOGOHEIGHT][LOGOWIDTH] ;
extern const uint16_t Renesas2[LOGOHEIGHT][LOGOWIDTH] ;
extern const uint16_t BLE_on[ICONHEIGHT][ICONWIDTH];
extern const uint16_t BLE_off[ICONHEIGHT][ICONWIDTH];
extern const uint16_t Splash[LCDHEIGHT][LCDWIDTH]; 
extern const int16_t shipcount[MINIY][MINIX];

#endif  SCREEN_H