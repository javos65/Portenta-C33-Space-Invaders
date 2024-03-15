/*****************************************************************************
* | File      	:	Debug.h
* | Function    :	debug with Serial printf and printfln
* | Info        : JV 2023
*----------------
* |	This version:   V2.0
* | Date        :   2023-01-16
* | Info        :   Basic version
*
******************************************************************************/
#ifndef __DEBUG_H
#define __DEBUG_H

#include <Arduino.h>
#include "stdio.h"

#define MYSERIAL Serial
#define DEBUG 1
#if DEBUG
	#define Debug(__info,...) MYSERIAL.print(__info,##__VA_ARGS__)
#else
	#define Debug(__info,...)  
#endif

#if DEBUG
	#define Debugln(__info,...) MYSERIAL.println(__info,##__VA_ARGS__)
#else
	#define Debugln(__info,...)  
#endif


#endif
