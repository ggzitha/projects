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


//_FOSC( OSC)


int i = 0;
int k = 0;

int main(int argc, char** argv) {

    //OSCCONbits.COSC = 0b010;    //  HSXT no PLL

    setupUART();
    for( i = 0; i < 10000000; i++ );

    U1TXREG = 'a';

    while(1);


    return (EXIT_SUCCESS);
}

void __attribute__(( __interrupt__, __auto_psv__ )) _U1TXInterrupt( void ){

    IFS0bits.U1TXIF = 0;        // Clear interrupt flag
    U1TXREG = 'a';
}

