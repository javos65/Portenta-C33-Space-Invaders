/*****************************************************************************
* | File      	:	TrueRandom.cpp
* | Function    :	use TRNG function of Cortex-series to make random generator
* | Info        : JV 2023
*----------------
* |	This version:   V1.0
* | Date        :   2024-03-16
* | Info        :   Basic version
* | Platform    :   Arduino Portenta H7 / C33 (STM32 M7/M4-core and REN CM33)
*
* TRNG fucntions used in TLS/SSL libraries
* STM32H7 : mbedTLS functions - fast
* R7FA6M5 : R_SCE_ HAL calls for random generator - slow
*
******************************************************************************/


#include "TrueRandom.h"

#if defined(ARDUINO_PORTENTA_C33)

#include "ssl_entropy.h" // used for function R_SCE_RandomNumberGenerate

uint32_t myTRNG ; // Define global 32 BIT location for HW TRNG function

// True Random Generator using ssl_entropy.h random generator function, returns fsp_error, places readnum number in
int _truerandom(uint32_t *number) 
{
fsp_err_t error=FSP_SUCCESS;
error = R_SCE_Open(&sce_ctrl, &sce_cfg);
if (!error)  error=R_SCE_RandomNumberGenerate(&myTRNG);
R_SCE_Close(&sce_ctrl);  
*number = myTRNG;
return ( (int) error);
}


// True Random Generator using ssl_entropy.h random generator function, returns fsp_error, places readnum number in
int _truerandom(unsigned char *buf,int len) 
{
fsp_err_t error=FSP_SUCCESS;
int lenc=0;
error = R_SCE_Open(&sce_ctrl, &sce_cfg);
while(lenc<len) {
    if (!error)  error=R_SCE_RandomNumberGenerate(&myTRNG);
    else break;
    for(int t=0;t<4;++t){
      buf[lenc]= (unsigned char) (myTRNG>>t)&0x000000ff;
      lenc++; 
      if ( !(lenc<len)) break;
      }
    }
R_SCE_Close(&sce_ctrl);  
return ( (int) error);
}

// True Random Generator using ssl_entropy.h random generator function, returns random number in 32 bits
uint32_t truerandom()
{
fsp_err_t error=FSP_SUCCESS;
error = R_SCE_Open(&sce_ctrl, &sce_cfg);
if (!error)  error=R_SCE_RandomNumberGenerate(&myTRNG);
else myTRNG=0;
R_SCE_Close(&sce_ctrl);  
return (myTRNG);
}

// True Random Generator using ssl_entropy.h random generator function, returns SIGNED random number in 32 bits (long) between (f)rom to (t)o
long truerandom(long f,long t)
{
int error=0;
long r;
  error = _truerandom(&myTRNG);
  if (!error )  {
        if (t==f) r = t; 
        if (t<f ) r = t + ((long) myTRNG&0xefffffff)%(f-t+1) ;
        if (t>f ) r = f + ((long) myTRNG&0xefffffff)%(t-f+1) ;
      }
  else r=0;
return(r);
}

#elif  defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4)
// ************************************* H7 LIBRARY BASED ON MBEDTLS *************************************************


#include "mbedtls/entropy.h"    // entropy settings
#include "mbedtls/ctr_drbg.h"   // trng generator

mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr_drbg;
const unsigned char pers[13]={"Arduino_Seed"};
uint32_t myTRNG ;                                 // Define global 32 BIT location for HW TRNG result store

// True Random Generator using mbedtls/ctr_drbg random generator function, returns fsp_error, places readnum number in
int _truerandom(uint32_t *number) // 32 bit generated number
{
int ret=0;
const unsigned char pers[5]={"Seed"};
unsigned char *pointer = (unsigned char*) number; // create char pointer to long number
mbedtls_ctr_drbg_init( &ctr_drbg );
mbedtls_entropy_init( &entropy );

if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy, pers, sizeof(pers) ) ) == 0 ) {
    mbedtls_ctr_drbg_random( &ctr_drbg,  pointer, 4 ); 
    }
else {
    *number=0;
    }
mbedtls_ctr_drbg_free( &ctr_drbg );
mbedtls_entropy_free( &entropy );
return ( ret);
}


// True Random Generator using mbedtls/ctr_drbg random generator function, returns fsp_error, places readnum number in
int _truerandom(unsigned char *buf,int len) // fill buffer with generated random numbers
{
int ret=0;
const unsigned char pers[5]={"Seed"};
mbedtls_ctr_drbg_init( &ctr_drbg );
mbedtls_entropy_init( &entropy );

if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy, pers, sizeof(pers) ) ) == 0 ) {
    mbedtls_ctr_drbg_random( &ctr_drbg,  buf, len ); 
    }
else {
    *buf=0;
    }
mbedtls_ctr_drbg_free( &ctr_drbg );
mbedtls_entropy_free( &entropy );
return ( ret);
}


// True Random Generator using mbedtls/ctr_drbg random generator function, returns random number in 32 bits
uint32_t truerandom()
{
_truerandom(&myTRNG);
return (myTRNG);
}

// True Random Generator using mbedtls/ctr_drbg random generator function, returns SIGNED random number in 32 bits (long) between (f)rom to (t)o
long truerandom(long f,long t)
{
long r=0;
int error=0;
  error = _truerandom(&myTRNG);
  if (!error )  {
        if (t==f) r = t; 
        if (t<f ) r = t + ((long) myTRNG&0xefffffff)%(f-t+1) ;
        if (t>f ) r = f + ((long) myTRNG&0xefffffff)%(t-f+1) ;
      }
  else r=0;
return(r);
}

#else 

#error !! Not a TRNG_function supported board, use PORTENTA H7 or PORTENTA C33 board with TRNG support !!

#endif