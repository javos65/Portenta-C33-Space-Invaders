#include <Arduino.h>
#include "TrueRandom.h"


void setup() {
Serial.begin(115200);
while(!Serial) ;
delay(1000);

int rerror;
uint32_t trng;
Serial.println("\nRandom test...1");
for(int i=0;i<8;++i) {
      rerror=_truerandom(&trng); // core function
    if(!rerror) { 
      Serial.print("Random 32bit :"); printHex( &trng,4);Serial.println();}
    else { 
      Serial.print("TRNG error :"); Serial.println(rerror,HEX);}
    }
delay(1000);


uint32_t bignumber[8];
Serial.println("\nRandom test...2");
for(int i=0;i<4;++i) {
      rerror=_truerandom( (unsigned char *) bignumber,sizeof(bignumber)); // core function
    if(!rerror) { 
      Serial.print("Random 256bit :"); printHex( bignumber,sizeof(bignumber)); Serial.println();
      }
    else { 
      Serial.print("TRNG error :"); Serial.println(rerror,HEX);}
    }
delay(1000);

Serial.println("\nRandom test...3");
long u,result;
Serial.print("[");
for( u=1;u<101;u++) {
    result=truerandom(-100,100); 
    Serial.print(result);
    if(u%10==0)Serial.print("]\n[");
    else Serial.print(",");
    }
  Serial.print("]\n");
  delay(1000);  



}

void loop() {

Serial.println("\nRandom test...4, press key and turn serial plotter on");
while (!Serial.available());
while(1) {
    long result=truerandom(-100,100); delay(10);
    Serial.print("100,-100,0,");Serial.println(result);
}
}


void printHex(uint32_t *data, uint8_t length) // prints 16-bit data in hex with leading zeroes
{
        char tmp[32];
        for (int i=0; i<length/4; i++) 
        { 
          sprintf(tmp, "%.8X",data[i]); 
          Serial.print(tmp); 
        }
}


