/*
 * This is a very very basic MIDI library.  This library can be used with any
 * MIDI IC without too much problems.  I have used the Sparkfun Music Instrument
 * Shield using the VS1053B IC with this library and found no problems...yet.
 * The uC used was a PIC18F4550 with a 4 MHz Crystal
 *
 * This library is in Public Domain ( BEERWARE License ).  If you like it,
 * you can buy me a beer if we ever meet.  I also accept cookies.
 *
 *
 * Documentation is found at http://meoworkshop.org/lib/midi
 *
 * Written by Allen Lee - alee@meoworkshop.org
 * 8-Sept/2012
 *
 * rev.2    Multichannel support
 *          Note & Rest Duration Functions
 *          Added txMIDI function to handle all MIDI Transmissions
 */

#include <p18f4550.h>
#include <xc.h>
#include <stdlib.h>
#include <delays.h>

//  Define something nice and short for the TRS status bit
#define TXF TXSTAbits.TRMT

//  Create some definitions for tempo settings
#define CLK 4000000     //  Define clock @ 4MHz


char atk = 60;
char rel = 80;
static char play = 0x90;
static char stp = 0x80;
static char ctrl = 0xb0;
static char bank_sel = 0x00;
static char vol_chg = 0x07;

//  Define some variables for tempo settings
//  char type is used because Delay functions accept only char
float whole = 0;
float half = 0;
float quarter = 0;
float eighth = 0;
float sixteenth = 0;

float incyc = 0;      //  Instruction cycles
float cpb = 0;        //  Cycles per beat


//  Sets up the uC EUSART for MIDI Transmission
//  See the datasheet for details on EUSART
void midi_init(void)
{
//  Set up PORTC bits to allow for EUSART
TRISCbits.RC7 = 1;      //  We must set RC 6 & 7 to 1 as defined in Datasheet
TRISCbits.RC6 = 1;

//  Set up EUSART Registers

TXSTAbits.TX9 = 0;      //  8-bit Transmission
TXSTAbits.SYNC = 0;     //  Async Mode
TXSTAbits.BRGH = 1;     //  High Speed Mode

RCSTAbits.SPEN = 1;     //  Enable Serial Port
RCSTAbits.RX9 = 0;      //  Keep RX 8-bit
RCSTAbits.CREN = 0;     //  Disable continuous receive

BAUDCONbits.TXCKP = 0;  //  Do NOT Invert Transmit Polarity
BAUDCONbits.RXDTP = 0;  //  Do NOT invert Receive Polarity
BAUDCONbits.BRG16 = 1;  //  16 bit Baud Generator
BAUDCONbits.WUE = 0;    //  Do not monitor RX Pin
BAUDCONbits.ABDEN = 0;  //  Disable Baud Rate Auto Detect

SPBRGH = 0b00000000;    //  n=31 for BR 31250.  Sets Register Pair for BRG
SPBRG = 0b00011111;

PIE1bits.TXIE = 0;      //  Disable Interrupts for USART

TXSTAbits.TXEN = 1;     //  Enable Transmit


}


//  Okay, now we start setting up MIDI Parameters

void txMIDI(char cmd, char data1, char data2)
{
    TXREG = cmd;
    while(!TXF);
    TXREG = data1;
    while(!TXF);

    //  Generally, commands < 0xb0 require 2 data bytes (data2)
    if((cmd & 0xF0) <= 0xB0)
    {
        TXREG = data2;
        while(!TXF);
    }

}

//  setBank() sets up the bank of instruments
//  0xB0 indicates control change while 0x00 means bank select

void setBank(char channel, char bank)
{
    txMIDI(((char)0xB0 | channel), (char)0x00, bank);

}

//  Function to set volume (up to 127)
//  0x07 indicates volume change
void setVol(char channel, char vol)
{
    txMIDI(((char)0xB0 | channel), (char)0x07, vol);
}

//  Function to set instrument in a bank
void setInstrument(char channel, char instr)
{
    txMIDI(((char)0xC0 | channel), instr, (char)0x00);
}


//  The next two functions are MIDI play & stop commands
//  This is where notes are actually played
void playNote(char channel, char note)
{
    txMIDI(((char)0x90 | channel), note, atk);
}

void stopNote(char channel, char note)
{
    txMIDI(((char)0x80 | channel), note, rel);
}


//  This function calculates the number of instruction cycles required for
//  a certain note or rest ("nr") duration.
//  NOTE:  I AM ONLY SUPPORTING QUARTER NOTE REFERENCE BEATS SO FAR.  DOTTED
//  QUARTERS / HALF NOTE REFERENCES WILL HOPEFULLY ADDED IN FUTURE REVISIONS
void setTempo(float tempo)
{
    //  Since 4 cycles = 1 instruction execution, @ 4MHz, 1000000 instructions
    //  pass each second.  incyc is the number of instructions per second
    //  cpb is "(instruction)cycles per beat"
    //  cpb also has a divide by 10000 because of Delay10KTCYx();
    //  Max argument passed to Delay10KTCYx() is 255, so slowest tempo is 94

    incyc = CLK / 4;
    cpb = ((60 / tempo) * incyc) / 10000;


    whole = 4 * cpb;
    half = 2 * cpb;
    quarter = cpb;
    eighth = cpb / 2;
    sixteenth = cpb / 4;
            
}


//  Here is where the magic happens for nr duration.
//  NOTE:  Delay10KTCYx() only accepts numbers up to 255!
void nr(char type)
{
    switch(type)
    {
        case 'w':
            Delay10KTCYx((char)whole);
            break;

        case 'h':
            Delay10KTCYx((char)half);
            break;

        case 'q':
            Delay10KTCYx((char)quarter);
            break;

        case 'e':
            Delay10KTCYx((char)eighth);
            break;

        case 's':
            Delay10KTCYx((char)sixteenth);
            break;
    
    }
}
