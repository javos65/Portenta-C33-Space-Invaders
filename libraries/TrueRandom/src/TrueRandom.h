/*****************************************************************************
* | File      	:	TrueRandom.h
* | Function    :	use TRNG function of Cortex-series to make random generator
* | Info        : JV 2023
*----------------
* |	This version:   V1.0
* | Date        :   2024-03-16
* | Info        :   Basic version
* | Platform    :   Arduino Portenta H7 / C33 (STM32 M7/M4-core and REN CM33)
*
******************************************************************************/

#ifndef __TRUERANDOM
#define __TRUERANDOM

#include "Arduino.h"

 extern int _truerandom(uint32_t *number);
 extern int  _truerandom(unsigned char *buf,int len) ;
 extern uint32_t truerandom();
 extern long truerandom(long f,long t);

#endif