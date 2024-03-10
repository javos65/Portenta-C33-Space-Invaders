
/*****************************************************************************
* | File      	:	StoreData.h
* | Function    :	Storing data on QSPI for C33 on LittleFileSystem
* | Info        : JV 2024
*----------------
* |	This version:   V1.0
* | Date        :   2024-01-16
* | Info        :   Basic version
*
******************************************************************************/

#ifndef __STOREDATA_H
#define __STOREDATA_H

#include <Arduino.h>

#define FS_NAME "qspi"
#define FOLDER_NAME "C33"
#define FILEHS "highscore.bin"
#define EEData_ID 0xB0        // EEprom /FFS save ID for object to retrieve


struct EEData {         
long time;
int highscore;
int highlevel;
uint8_t identity;
int counter;
uint8_t stop; // dummy
};

extern int      Mount_Qspi();
extern int      Delete_File();
extern int      Check_Filesize();
extern uint8_t  Read_Data(struct EEData *O);
extern void     Debug_Buffer(char* b,int s);
extern uint8_t  Save_Data(struct EEData *O);
extern uint8_t  Debug_Data(struct EEData *O);

extern void     Read_Highscore(int* HS, int* HL);
extern void     Write_Highscore(int  HS, int HL);

#endif