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

typedef struct{

	char buffer[100];

	char const *bufPtr;
	volatile char *bufIndex; 
	volatile char *freeStartPtr;
	volatile char *freeEndPtr;

	volatile unsigned int freeSpace;

        volatile int bufEmpty;
	

} ringBuf;

int setupAnalog( unsigned int );
int setupUART();

static unsigned int tickCount;
static unsigned int brgValue;

#endif	/* SEISMEOWMETERFW_H */

