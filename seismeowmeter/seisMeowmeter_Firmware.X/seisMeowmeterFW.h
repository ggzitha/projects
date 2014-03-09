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
#define ARRAYSIZE(a) ( sizeof(a) / sizeof(a[0] )) 	//	Compilers HATE this...but why??

/* 	Okay, ring buffers are great and all but they have one caveat.
	If the Index and Free Start Pointers are the same then the buffer is either empty or full
	The problem is how can we tell them apart?
	This implementation takes into account the fact that the UART ISR empties the buffer while the addToBuffer() fcn
	fills it.  If the ISR bufIndex appraches freeStartPtr, then the buffer is empty.
	Alternatively, in the addToBuffer() fcn, if the freeStart Ptr = bufIndex, we have a buffer full situation.

	In summary, there is NO built in method to determine full/empty buffers.  It's up to YOU to figure that out
*/

typedef struct{

    char buffer[BUFSIZE];

    char *bufPtr;
    volatile char *bufIndex;
    volatile char *freeStartPtr;
    volatile unsigned int freeSpace;
    volatile int bufEmpty;
    volatile int bufFull;

} ringBuf_t;

static unsigned int tickCount;
static unsigned int brgValue;

int setupAnalog( unsigned int );
int setupUART(); 
int addStrToBuffer( ringBuf_t*, char* );
int addNumToBuffer( ringBuf_t, unsigned long int );

#endif	/* SEISMEOWMETERFW_H */

