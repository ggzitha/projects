/*
 * File:   main.c
 * Author: superkittens
 *
 * Created on February 16, 2014, 2:03 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "seisMeowmeterFW.h"
#include "debug.h"

#define BYTE 1

enum{ RELEASE, LOCK };

//  Transmit ring buffer variables
int rbuf_lock = RELEASE;
volatile int uartActiveFlag = 0;
volatile ringBuf_t uartBuf;
volatile ringBuf_t *puBuf = &uartBuf;
volatile unsigned char target;
unsigned char *ptarget = &target;
volatile int rbuf_readFlag = 0;

//  Receive ring buffer variables
volatile ringBuf_t cmdBuf;
volatile ringBuf_t *pcmdBuf = &cmdBuf;
volatile unsigned char cmdTarget;
unsigned char *pcmdTarget = &cmdTarget;

int i = 0;

void check_uart_active();

//  External HS XT Config bits
_FOSC( POSCMD_HS & OSCIOFNC_OFF );
_FOSCSEL( FNOSC_PRI & IESO_OFF );
_FWDT( FWDTEN_OFF );

int main(int argc, char** argv) {


    OSCCONbits.COSC = 0b010;    //  HSXT no PLL
    OSCCONbits.CLKLOCK = 1;
    OSCCONbits.IOLOCK = 1;

    CLKDIVbits.ROI = 0;
    CLKDIVbits.DOZE = 0b000;
    CLKDIVbits.DOZEN = 1;

    TRISBbits.TRISB8 = 0;
    LATBbits.LATB8 = 1;

    buf_create(pcmdBuf);
    buf_create(puBuf);

    setupUART();

    unsigned char *test = "will the circle be unbroken by and by?\n";
    unsigned char *test2 = "is a greater home awaiting in the sky?\n";
    buf_write(puBuf, test, strlen(test));
    
    check_uart_active();

    while(1){


        if(readCommand(pcmdBuf) == 3){
            rbuf_lock = LOCK;
            buf_write(puBuf, test2, strlen(test2));
            rbuf_lock = RELEASE;

            check_uart_active();

        }


        /*
        rbuf_lock = LOCK;
        buf_write(puBuf, test2, strlen(test2));
        rbuf_lock = RELEASE;

        check_uart_active();
        */

    }


    return (EXIT_SUCCESS);
}

void check_uart_active(){

    if(uartActiveFlag == 0){
        uartActiveFlag = 1;
        buf_read(puBuf, ptarget, BYTE);
        U1TXREG = *ptarget;
    }
}

//  UART TX ISR
void __attribute__(( __interrupt__, __auto_psv__ )) _U1TXInterrupt( void ){

    IFS0bits.U1TXIF = 0;

    //  When the buf_write function is running, it is crucial that the ISR does not run the buf_read() function because the start and end addresses may potentially become corrupted!
    //  This is why we have a lock, rbuf_lock
    switch(rbuf_lock){

        case LOCK:
            uartActiveFlag = 0;
            break;

        case RELEASE:
            rbuf_readFlag = buf_read(puBuf, ptarget, BYTE);

            //  buf_read returns -1 if there is no more data to be read, -2 if something went horribly wrong
            if(rbuf_readFlag < 0){
                uartActiveFlag = 0;
                break;
            }

            else{
                U1TXREG = *ptarget;
                break;
            }

        default:
            break;
    }

}

void __attribute__(( __interrupt__, __auto_psv__ )) _U1RXInterrupt( void ){

    IFS0bits.U1RXIF = 0;

    cmdTarget = U1RXREG;

    if(LATBbits.LATB8 == 0)
        LATBbits.LATB8 = 1;

    else if(LATBbits.LATB8 == 1)
        LATBbits.LATB8 = 0;
    
    buf_write(pcmdBuf, pcmdTarget, BYTE);


}


