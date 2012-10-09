/* 
 * Quick Side Project
 * OOT Jukebox.  Select one of four OOT songs with a rot encoder and an LED
 * matrix.  Sound is made by the VS1053b (Sparkfun Music Instrument Shield)
 *
 * All Ocarina of Time songs were created by Koji Kondo.
 * 
 *
 * You can contact me if you have any questions or comments at
 * alee@meoworkshop.org
 *
 * For Pranai
 *
 * Issues:  The rotary encoder does not play nice.  Trying to go counter clockwise
 *          results in the LEDs acting a bit erratic (ex. turning the encoder
 *          counter-clockwise to go from LED 3 to LED 2 might actually result
 *          in LED 1 turning on instead).  I'm thinking this might be because
 *          I didn't have any debouncing circuitry put in.
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f4550.h>
#include <delays.h>
#include "midi.h"
#include "oot_bank.h"

//  Define LED Matrix Pins
#define LED1_TRIS TRISDbits.TRISD0
#define LED2_TRIS TRISDbits.TRISD1
#define LED3_TRIS TRISDbits.TRISD2
#define LED4_TRIS TRISDbits.TRISD3

#define LED1 LATDbits.LATD0
#define LED2 LATDbits.LATD1
#define LED3 LATDbits.LATD2
#define LED4 LATDbits.LATD3

//  Define Rot Enc Pins
#define SWA_TRIS TRISBbits.TRISB2
#define SWA PORTBbits.RB2
#define SWB_TRIS TRISBbits.TRISB3
#define SWB PORTBbits.RB3
#define SW_TRIS TRISBbits.TRISB4
#define SW PORTBbits.RB4


//  Set configuration to external oscillator, no WatchDog TImer
#pragma config FOSC=XT_XT, WDT=OFF, IESO=ON
#pragma code low_vector=0x18

volatile char led[4] = {0,0,0,0};
volatile int index = 0;
char bank = 0x00;
char vol_0 = 80;
char vol_1 = 70;
char vol_2 = 80;

void lp_int();      //  Declare interrupt function prototype

//  The way interrupts are handled in the C18 toolchain is described in the help
//  section in MPLabX.  
void low_interrupt()
{
    _asm GOTO lp_int _endasm
}

#pragma code
#pragma interruptlow lp_int


void main(void) {

    ADCON1 = 0b00001111;    //  Ensure ALL Analog pins are digital

    //  Set Interrupt Registers Here
    INTCON2bits.INTEDG2 = 1;    //  Detect Rising Edge
    RCONbits.IPEN = 1;          //  Enable Priority Levels
    INTCON3bits.INT2F = 0;      //  Clear Interrupt Flag
    INTCON3bits.INT2IE = 1;     //  Enable INT2 Interrupt
    INTCON3bits.INT2IP = 0;     //  Set Interrupt to low priority
    INTCONbits.PEIE = 1;        //  Enable Peripheral Interrupts
    INTCONbits.GIE = 1;         //  Global Interrupt Enable

    //  Set LED Pins to Output
    LED1_TRIS = 0;
    LED2_TRIS = 0;
    LED3_TRIS = 0;
    LED4_TRIS = 0;

    //  Set the first LED on as the program starts up
    LED1 = 1;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;

    //  Set RotEnc Pins to Input
    SWA_TRIS = 1;
    SWB_TRIS = 1;
    SW_TRIS = 1;

    //  Set up initial parameters for MIDI
    midi_init();
    setBank(0,bank);
    setBank(1,bank);
    setBank(2,bank);
    setVol(0,vol_0);
    setVol(1,vol_1);
    setVol(2,vol_2);



    //Start Infinite Loop here
    while(1)
    {
        //  Play music if switch is pressed
        if(SW == 0)
        {
           //  Delay is to let the debounce run its course
            Delay10KTCYx(10);
            switch(index)
            {
                case 0:
                    forest();
                    break;
                case 1:
                    storms();
                    break;

                case 2:
                    bolero();
                    break;

                case 3:
                    prelude();
                    break;
            }

            //  If volume has been messed around in the songs, reset them
            setVol(0,vol_0);
            setVol(1,vol_1);
            setVol(2,vol_2);

        }


    }


}

void lp_int()
{
    //  Rotary Encoder Interrupt goes here
    if(INTCON3bits.INT2IF == 1)
    {
        //  Delay for debounce
        Delay10KTCYx(5);

        //  Turn off all LEDs
        led[0] = 0;
        led[1] = 0;
        led[2] = 0;
        led[3] = 0;

        LED1 = led[0];
        LED2 = led[1];
        LED3 = led[2];
        LED4 = led[3];


        //  Encoder has moved clockwise if SWA=SWB
        if(SWA == SWB)
        {
            index++;
            //  Turn on the selected LED
            led[index] = 1;
        }

        //  Encoder has moved counter-clockwise if SWA does not equal SWB
        else if(SWA != SWB)
        {
            index--;
            led[index] = 1;
        }

        //  If encoder goes past LED #3, revert to LED #0
        if(index > 4)
        {
            index = 0;
        }

        //  If encoder goes under LED #0, go to LED #3
        else if(index < 0)
        {
            index = 3;
        }

        //  Turn on selected LED
        LED1 = led[0];
        LED2 = led[1];
        LED3 = led[2];
        LED4 = led[3];

        INTCON3bits.INT2IF = 0;      //  Clear Interrupt Flag

    }
}

