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

    IFS0bits.U1TXIF = 0;            // Clear UART interrupt flag
    IEC0bits.U1TXIE = 1;            // Enable UART interrupts

    RPOR7bits.RP15R = 0b000011;     //  Set UART TX Pin to RP15

    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;

    return 0;

}

//  Not overly sure if this function is all that necessary...
int buf_create(ringBuf_t *buf){

    buf->length = BUFSIZE + 1;
    buf->start = 0;
    buf->end = 0;

    if(buf == NULL)
        return -1;

    else
        return 0;
}

int buf_write(ringBuf_t *buf, char *data, int length){

    if(buf_availableData(buf) == 0){
        buf->start = buf->end = 0;
    }

    if(buf_availableSpace(buf) < length)
        return -1;

    void *result = memcpy(RINGBUF_ENDS_AT(buf), data, length);
    if(result == NULL){
        return -2;
    } 

    buf->end = (buf->end + length) % buf->length;

    return 0;

}

int buf_read(ringBuf_t *buf, char *target, int amount){

    if(amount > buf_availableData(buf))
        return -1;

    void *result = memcpy(target, RINGBUF_STARTS_AT(buf), amount);

    if(result == NULL)
        return -2;

    buf->start = (buf->start + amount) % buf->length;

    if(buf->end == buf->start)
        buf->start = buf->end = 0;

    return 0;
}

int buf_availableData(ringBuf_t *buf){
    return (buf->end + 1) % buf->length - buf->start - 1;
}

int buf_availableSpace(ringBuf_t *buf){
    return buf->length - buf->end -1;
}
