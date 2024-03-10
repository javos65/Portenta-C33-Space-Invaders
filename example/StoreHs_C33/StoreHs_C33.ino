/*
  Portenta C33 - Test QSPI Flash with Little FS

  The sketch shows how to mount an QSPI and list its content.
  The circuit:  - Portenta C33 + Portenta Breakout 

Target is to create an maintain this file : /qspi/C33/highscore.bin, and write a Data Structure in raw bytes.

*/
#include "StoreData.h"


EEData G_Object = {12443345,50,2,EEData_ID,10,0}; // Global Variable Object structure example
EEData T_Object =  {0,0,0,0x33,0,0}; ;

void setup() {
  Serial.begin(115200);
  while(!Serial) {}
}


void loop() {

 Debug_Data(&T_Object);
 Debug_Data(&G_Object);Serial.println();

 Mount_Qspi();
 Read_Data(&T_Object);
 Debug_Data(&T_Object);Serial.println();

 Mount_Qspi();
 G_Object.highscore+=10; G_Object.time+=1131313;
 Debug_Data(&G_Object);Serial.println();
 Save_Data(&G_Object);

 Mount_Qspi();
 Read_Data(&T_Object);
 Debug_Data(&T_Object);Serial.println();
delay(5000);


}
