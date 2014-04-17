/*
 * File:   main.c
 * Author: superkittens
 *
 * Created on February 16, 2014, 2:03 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "seisMeowmeterFW.h"

#define BYTE 1

enum{ RELEASE, LOCK };

int rbuf_lock = 0;
volatile int uartActiveFlag = 0;
volatile ringBuf_t uartBuf;
volatile ringBuf_t *uBuf = &uartBuf;
volatile char target;
char *rbuf_target = &target;
volatile int rbuf_readFlag = 0;

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

    buf_create(uBuf);

    char *test = "will the circle be unbroken by and by?\n";
    char *test2 = "is a greater home awaiting in the sky?\n";
    buf_write(uBuf, test, strlen(test));
    setupUART();

    check_uart_active();

    while(1){

        rbuf_lock = LOCK;
        buf_write(uBuf, test2, strlen(test2));
        rbuf_lock = RELEASE;

        check_uart_active();

        for(i=0; i < 10000; i++);

    }


    return (EXIT_SUCCESS);
}

void check_uart_active(){

    if(uartActiveFlag == 0){
        uartActiveFlag = 1;
        buf_read(uBuf, rbuf_target, BYTE);
        U1TXREG = *rbuf_target;
    }
}

//  UART ISR
void __attribute__(( __interrupt__, __auto_psv__ )) _U1TXInterrupt( void ){

    IFS0bits.U1TXIF = 0;

    //  When the buf_write function is running, it is crucial that the ISR does not run the buf_read() function because the start and end addresses may potentially become corrupted!
    //  This is why we have a lock, rbuf_lock
    switch(rbuf_lock){

        case LOCK:
            uartActiveFlag = 0;
            break;

        case RELEASE:
            rbuf_readFlag = buf_read(uBuf, rbuf_target, BYTE);
            rbuf_target;

            //  buf_read returns -1 if there is no more data to be read, -2 if something went horribly wrong
            if(rbuf_readFlag < 0){
                uartActiveFlag = 0;
                break;
            }

            else{
                U1TXREG = *rbuf_target;
                break;
            }

        default:
            break;
    }

}


