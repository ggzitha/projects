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
#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/16)-1
#define T1PRESCALE 64


int setupAnalog( unsigned int );
int setupUART();

static unsigned int tickCount;
static unsigned int brgValue;

#endif	/* SEISMEOWMETERFW_H */

