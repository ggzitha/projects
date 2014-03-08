/* 
 * File:   main.c
 * Author: superkittens
 *
 * Created on February 16, 2014, 2:03 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "seisMeowmeterFW.h"


//  External HS XT Config bits
_FOSC( POSCMD_HS & OSCIOFNC_OFF );
_FOSCSEL( FNOSC_PRI & IESO_OFF );
_FWDT( FWDTEN_OFF );

ringBuf_t uBuf;

int main(int argc, char** argv) {

    OSCCONbits.COSC = 0b010;    //  HSXT no PLL
    OSCCONbits.CLKLOCK = 1;
    OSCCONbits.IOLOCK = 1;

    CLKDIVbits.ROI = 0;
    CLKDIVbits.DOZE = 0b000;
    CLKDIVbits.DOZEN = 1;

    int i = 0;
    char *test = "Will the circle be unbroken by and by, by and by?\n";

    initUBuffer( uBuf );
    setupUART();

    while(1){

    }


    return (EXIT_SUCCESS);
}

//  UART ISR
void __attribute__(( __interrupt__, __auto_psv__ )) _U1TXInterrupt( void ){

    if( uBuf.bufEmpty == 1 ){ /* Nothing to do here*/ }

    else{

        IFS0bits.U1TXIF = 0;

        U1TXREG = *( uBuf.bufIndex );       //  Should this belong here???
        uBuf.freeEndPtr = uBuf.bufIndex;

        //  Recalculate free space
        //  There are three cases: start pointer > end pointer, the other way around and them being equal
        if( uBuf.freeStartPtr > uBuf.freeEndPtr ){ 
            uBuf.freeSpace = ( BUFSIZE - (int)uBuf.freeStartPtr ) - ( uBuf.freeEndPtr - uBuf.bufPtr );
        }

        else if( uBuf.freeStartPtr < uBuf.freeEndPtr ){
            uBuf.freeSpace = ( uBuf.freeEndPtr - uBuf.freeStartPtr ) + 1;
        }

        //  If freeStartPtr = freeEndPtr, this means there is one byte of free space
        //  The ISR guarantees at least one free byte of space
        else{
            uBuf.freeSpace = 1;
        }

        uBuf.bufIndex++;


        //  If index exceeds the size of the buffer, reset it back to the start...because, you know...circle.
        if( uBuf.bufIndex >= ( uBuf.bufPtr + BUFSIZE )){ uBuf.bufIndex = uBuf.bufPtr; }
        //  If the index arrives at the start of free space, our buffer is empty
        if( uBuf.bufIndex == uBuf.freeStartPtr ){ uBuf.bufEmpty = 1; }

    }
}

