/*
 * File:   seisMeowmeterFW.h
 * Author: superkittens
 *
 * Created on February 16, 2014, 1:39 PM
 */
#include <xc.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifndef SEISMEOWMETERFW_H
#define	SEISMEOWMETERFW_H
#define FCY 8000000
#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/16)-1
#define T1PRESCALE 64
#define BUFSIZE 256
#define RINGBUF_STARTS_AT(B) ((B)->buffer + (B)->start)
#define RINGBUF_ENDS_AT(B) ((B)->buffer + (B)->end)

/* 	
    The ring buffer implementation is based off of Zed Shaw's exercise (44) in "Learn C the Hard Way"
    You can see it at: http://c.learncodethehardway.org/book/ex44.html

    For details on this particular ring buffer implementation visit [meoWS link]
*/

typedef struct{
    unsigned char buffer[BUFSIZE];
    int length;
    volatile int start;
    volatile int end;
} ringBuf_t;

static unsigned int tickCount;
static unsigned int brgValue;

int setupAnalog( unsigned int );
int setupUART();
int readCommand(ringBuf_t *buf);

//  Ring Buffer functions
int buf_create(ringBuf_t *buf);
int buf_write(ringBuf_t *buf, unsigned char *data, int length);
int buf_read(ringBuf_t *buf, unsigned char *target, int amount);
int buf_availableData(ringBuf_t *buf);
int buf_availableSpace(ringBuf_t *buf);


#endif	/* SEISMEOWMETERFW_H */

