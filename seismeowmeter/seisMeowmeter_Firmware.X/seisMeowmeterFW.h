/* 
 * File:   seisMeowmeterFW.h
 * Author: superkittens
 *
 * Created on February 16, 2014, 1:39 PM
 */
#include <xc.h>

#ifndef SEISMEOWMETERFW_H
#define	SEISMEOWMETERFW_H
#define FCY 8000000
#define BAUDRATE 19200
#define BRGVAL ((FCY/BAUDRATE)/16)-1
#define T1PRESCALE 64
#define BUFSIZE 256
#define ARRAYSIZE(a) ( sizeof(a) / sizeof(a[0] )) 

enum{ DEC, STR };

typedef struct{

    char buffer[BUFSIZE];

    char *bufPtr;
    volatile char *bufIndex;
    volatile char *freeStartPtr;
    volatile char *freeEndPtr;

    volatile unsigned int freeSpace;
    volatile int bufEmpty;

} ringBuf_t;

static unsigned int tickCount;
static unsigned int brgValue;

int setupAnalog( unsigned int );
int setupUART(); 
int initUBuffer( ringBuf_t );
int addStrToBuffer( ringBuf_t, char* );
int addNumToBuffer( ringBuf_t, unsigned long int );

#endif	/* SEISMEOWMETERFW_H */

