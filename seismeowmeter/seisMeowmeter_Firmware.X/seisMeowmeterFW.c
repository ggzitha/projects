#include "seisMeowmeterFW.h"

int setupAnalog( unsigned int sampleRate ){

    tickCount = FCY / ( T1PRESCALE * sampleRate );

    //  Set ADC Registers

    ADCONbits.ADON = 0;             //  Turn off ADC for configuration
    ADCONbits.ADSIDL = 1;
    ADCONbits.SLOWCLK = 0;
    ADCONbits.GSWTRG = 1;
    ADCONbits.FORM = 0;
    ADCONbits.EIE = 0;
    ADCONbits.ORDER = 1;
    ADCONbits.SEQSAMP = 0;
    ADCONbits.ASYNCSAMP = 0;

    ADCONbits.ADCS2 = 0;            //  Default ADC Clock Frequency
    ADCONbits.ADCS1 = 1;
    ADCONbits.ADCS1 = 1;

    ADPCFGbits.PCFG0 = 0;            //  Make sure that pins AN0 and 1 are analog
    ADPCFGbits.PCFG1 = 0;

    ADCPC0bits.IRQEN0 = 1;           //  We are going with interrupts here so let's enable them!
    ADCPC0bits.SWTRG0 = 1;
    ADCPC0bits.TRGSRC0 = 0b01100;    //  Interrupt trigger source -> TMR1 Match


    //  Set TMR1 Registers
    T1CONbits.TON = 0;              //  Shut off timer for configuration
    T1CONbits.TSIDL = 1;
    T1CONbits.TGATE = 0;
    T1CONbits.TCKPS = 0b10;         //  1:64 Prescaler
    T1CONbits.TCS = 0;              //  Use internal clock
    TMR1= 0x0000;
    PR1 = tickCount;

    T1CONbits.TON = 1;              //  Turn on Timer
    ADCONbits.ADON = 1;             //  Turn on ADC



    return 0;

}

int setupUART(){

    //brgValue = ( FCY / ( 16 * baudRate )) - 1;

    U1MODEbits.UARTEN = 0;
    U1STAbits.UTXEN = 0;

    U1MODEbits.USIDL = 0;
    U1MODEbits.IREN = 0;
    U1MODEbits.RTSMD = 1;
    U1MODEbits.UEN = 0b00;
    U1MODEbits.WAKE = 0;
    U1MODEbits.LPBACK = 0;
    U1MODEbits.ABAUD = 0;
    U1MODEbits.URXINV = 0;
    U1MODEbits.BRGH = 0;            //  Enable low speed mode
    U1MODEbits.PDSEL = 0b00;
    U1MODEbits.STSEL = 0;

    U1STAbits.UTXISEL1 = 0;         
    U1STAbits.UTXISEL0 = 0;
    U1STAbits.UTXINV = 0;
    U1STAbits.UTXBRK = 0;
    

    U1BRG = BRGVAL;

   // IFS0bits.U1TXIF = 0;            // Clear UART interrupt flag
    IEC0bits.U1TXIE = 1;            // Enable UART interrupts

    RPOR7bits.RP15R = 0b000011;     //  Set UART TX Pin to RP15

    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;

    return 0;

}

int initUBuffer( ringBuf_t uBuf ){

    uBuf.bufPtr = uBuf.buffer;
    uBuf.bufIndex = uBuf.bufPtr;
    uBuf.freeStartPtr = uBuf.bufPtr;
    uBuf.freeEndPtr = uBuf.bufPtr;
    uBuf.freeSpace = BUFSIZE;
    uBuf.bufEmpty = 1;

    return 0;

}

int addStrToBuffer( ringBuf_t uBuf, char *data ){

    int arraySize = sizeof( data ) / sizeof( data[0] );
    int i = 0;
    
    //  Kick out of function if no space is available in the buffah
    if( arraySize > uBuf.freeSpace ) { return -1; }

    else if( arraySize <= uBuf.freeSpace ){

        char temp = *data;
        char *tempFreeStartPtr = uBuf.freeStartPtr;

        //  All strings end with a null (\0) character so this is how we'll know when to stop adding data to the buffer
        while(temp != '\0'){
            //  Add one character to the first empty slot
            *(uBuf.freeStartPtr) = *(data + i );
            uBuf.freeStartPtr++;
            uBuf.freeSpace--;

            if( uBuf.freeStartPtr > ( uBuf.bufPtr + BUFSIZE )) { uBuf.freeStartPtr = uBuf.bufPtr; }
            i++;

        }

        //  If buffer is empty then start the UART transmission again by adding data to the UART buffer
        if( uBuf.bufEmpty == 1 ){
            U1TXREG = *(tempFreeStartPtr);
        }

        return 0;
    }

}

int addNumToBuffer( ringBuf_t uBuf, unsigned long int data ){

    int i = 0;

    char buf[8 * sizeof(long) + 1];
    char * str = &buf[sizeof(buf) - 1];

    *str = '\0';

    do{
        unsigned long m = data;
        data /= 10;
        char c = m - (10 * data);

        *(--str) = c + 48;      //  Offset by 48 to convert from straight numbers to ascii numbers
    }while(data);

    addStrToBuffer( uBuf, buf );

    return 0;
}
