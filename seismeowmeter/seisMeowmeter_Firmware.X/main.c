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

volatile ringBuf_t uTBuf;

int main(int argc, char** argv) {

    uTBuf.bufPtr = uTBuf.buffer;
    uTBuf.bufIndex = uTBuf.bufPtr;
    uTBuf.freeStartPtr = uTBuf.bufPtr;
    uTBuf.freeSpace = BUFSIZE;
    uTBuf.bufEmpty = 1;
    uTBuf.bufFull = 0;

    ringBuf_t *ptr = &uTBuf;


    OSCCONbits.COSC = 0b010;    //  HSXT no PLL
    OSCCONbits.CLKLOCK = 1;
    OSCCONbits.IOLOCK = 1;

    CLKDIVbits.ROI = 0;
    CLKDIVbits.DOZE = 0b000;
    CLKDIVbits.DOZEN = 1;

    int i = 0;
    char *test = "will the circle be unbroken by and by, by and by?";

    setupUART();
    addStrToBuffer( ptr, test );

    while(1){
        //
        for(i = 0; i < 1000; i++ );
        addStrToBuffer( ptr, test );
    }


    return (EXIT_SUCCESS);
}

//  UART ISR
void __attribute__(( __interrupt__, __auto_psv__ )) _U1TXInterrupt( void ){
    
/*
    if( uTBuf->bufEmpty == 1 ){
        IFS0bits.U1TXIF = 0;
    }

    else if( uTBuf->bufEmpty == 0 ){

        U1TXREG = *( uTBuf->bufIndex );       //  Should this belong here???
        uTBuf->bufIndex++;

        //  If index exceeds the size of the buffer, reset it back to the start...because, you know...circle.
        if( uTBuf->bufIndex >= ( uTBuf->bufPtr + BUFSIZE )){ uTBuf->bufIndex = uTBuf->bufPtr; }
        //  If the index arrives at the start of free space, our buffer is empty
        //if( uTBuf.bufIndex == uTBuf.freeStartPtr ){ uTBuf.bufEmpty = 1; }
        if( uTBuf->bufFull == 1 ){ uTBuf->bufFull = 0; }

        //  Recalculate free space
        //  There are three cases: start pointer > end pointer, the other way around and them being equal
        if( uTBuf->freeStartPtr > uTBuf->bufIndex ){
            uTBuf->freeSpace = ( BUFSIZE - (int)uTBuf->freeStartPtr ) - ( uTBuf->bufIndex - uTBuf->bufPtr );
        }

        else if( uTBuf->freeStartPtr < uTBuf->bufIndex ){
            uTBuf->freeSpace = ( uTBuf->bufIndex - uTBuf->freeStartPtr ) + 1;
        }

        //  If freeStartPtr = freeEndPtr, this means there is one byte of free space
        //  The ISR guarantees at least one free byte of space
        else{
            uTBuf->freeSpace = 1;
        }

        IFS0bits.U1TXIF = 0;


    }*/

    
    if( uTBuf.bufEmpty == 1 ){
        IFS0bits.U1TXIF = 0;

    }

    else if( uTBuf.bufEmpty == 0 ){

        U1TXREG = *( uTBuf.bufIndex );       //  Should this belong here???
        uTBuf.bufIndex++;

        //  If index exceeds the size of the buffer, reset it back to the start...because, you know...circle.
        if( uTBuf.bufIndex >= ( uTBuf.bufPtr + BUFSIZE )){ uTBuf.bufIndex = uTBuf.bufPtr; }
        //  If the index arrives at the start of free space, our buffer is empty
        if( uTBuf.bufIndex == uTBuf.freeStartPtr ){ uTBuf.bufEmpty = 1; }
        if( uTBuf.bufFull == 1 ){ uTBuf.bufFull = 0; }

        //  Recalculate free space
        //  There are three cases: start pointer > end pointer, the other way around and them being equal
        if( uTBuf.freeStartPtr > uTBuf.bufIndex ){
            uTBuf.freeSpace = ( BUFSIZE - (int)uTBuf.freeStartPtr ) - ( uTBuf.bufIndex - uTBuf.bufPtr );
        }

        else if( uTBuf.freeStartPtr < uTBuf.bufIndex ){
            uTBuf.freeSpace = ( uTBuf.bufIndex - uTBuf.freeStartPtr ) + 1;
        }

        //  If freeStartPtr = freeEndPtr, this means there is one byte of free space
        //  The ISR guarantees at least one free byte of space
        else{
            uTBuf.freeSpace = 1;
        }

        IFS0bits.U1TXIF = 0;


    }
}

