/*	This is the firmware for the Pillbox Thermometer.
 *	It uses the TMP36 temperature sensor from Analog Devices and the AtTiny84 uC
 *	The ADC reads the temperature and lights up a corresponding LED.
 *	There are 10 LEDs ranging from -15 to 25 C
 *
 *	Created by: Allen Lee (alee@meoworkshop.org)
 *	Rev: rev.O
 *
 *	http://meoworkshop.org/a-pillbox-thermometer
 *
 */

//  We want to run on low power so clock is set to 1 MHz
#define F_CPU 1000000
#define HIGH 1
#define LOW 0


#include <avr/io.h>
#include <util/delay.h>

//  Create definitions of GPIO Ports
enum{P0, P1, P2, P3, P4, P5, P6, P7, P8, P9};

float voltage;
float adc_temp;
uint16_t aval;
uint8_t i = 0;


//  Low and High ADC values
uint8_t low;
uint8_t high;



void init(void)
{
    
	//  Setup ADC
	//  Disable digital pin mode for ADC pin 0
	DIDR0 |= (ADC0D);
	ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS2);
    ADCSRB |= (0 << ADLAR);

	//  Setup LED Pins
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
	DDRA |= (1 << PA1) | (1 << PA2) | (1 << PA3) | (1 << PA4) | (1 << PA5) | (1 << PA6) | (1 << PA7);

}

//  Not to be confused with "digitalWrite()" in the Arduino environment, but it pretty much does the same thing anyway
void digitalWrite(uint8_t port, uint8_t state)
{
	switch(port)
	{
		//  Pins are numbered according to how the uC and LEDs are arranged so P0 may not necessairly be Port B0

		// Port P0 -> PB2
		case 0:
			if(state == LOW)
				PORTB &= (0 << PB2);

			else if(state == HIGH)
				PORTB |= (1 << PB2);

		break;

		// Port P1 -> PA7
		case 1:
			if(state == LOW)
				PORTA &= (0 << PA7);

			else if(state == HIGH)
				PORTA |= (1 << PA7);

		break;

		//  Port P2 -> PA6
		case 2:
			if(state == LOW)
				PORTA &= (0 << PA6);
			
			else if(state == HIGH)
				PORTA |= (1 << PA6);

		break;

		//  Port P3 -> PA5
		case 3:
			if(state == LOW)
				PORTA &= (0 << PA5);

			else if(state == HIGH)
				PORTA |= (1 << PA5);

		break;

		//  Port P4 -> PA4
		case 4:
			if(state == LOW)
				PORTA &= (0 << PA4);

			else if(state == HIGH)
				PORTA |= (1 << PA4);

		break;

		//  Port P5 -> PA3
		case 5:
			if(state == LOW)
				PORTA &= (0 << PA3);

			else if(state == HIGH)
				PORTA |= (1 << PA3);

		break;

		//  Port P6 -> PA2
		case 6:
			if(state == LOW)
				PORTA &= (0 << PA2);

			else if(state == HIGH)
				PORTA |= (1 << PA2);

		break;

		//  Port P7 -> PA1
		case 7:
			if(state == LOW)
				PORTA &= (0 << PA1);

			else if(state == HIGH)
				PORTA |= (1 << PA1);

		break;

		//  Port P8 -> PB0
		case 8:
			if(state == LOW)
				PORTB &= (0 << PB0);

			else if(state == HIGH)
				PORTB |= (1 << PB1);

		break;

		//  Port P9 -> PB1
		case 9:
			if(state == LOW)
				PORTB &= (0 << PB1);

			else if(state == HIGH)
				PORTB |= (1 << PB1);

		break;

	}
}



void main(void)
{
	init();

	while(1)
	{
        
        //  Start ADC Conversion
        ADCSRA |= (1 << ADSC);
        
        //  Loop continuously until conversion completes
        while(ADCSRA & (1 << ADSC));
        
        //  Put ADC Results into variables and combine them all into one variable
        
        low = ADCL;
        high = ADCH;
		aval = (high << 8) | low;
        
        //  Convert raw ADC values to actual temperatures
        voltage = ((float)aval * 3) / 1023;
        adc_temp = (voltage - 0.5) / 0.01;
        
        //  Turn off all LEDs
        for(i = 0; i < 10; i++)
        {
            digitalWrite(i, LOW);
        }
        
        //  Test the temperture and light the appropriate LED
        if(adc_temp <= -15)
            digitalWrite(9, HIGH);
        
        if((adc_temp <= -10) && (adc_temp > -15))
            digitalWrite(8, HIGH);
        
        if((adc_temp <= -5) && (adc_temp > -10))
            digitalWrite(7, HIGH);
        
        if((adc_temp <= 0) && (adc_temp > -5))
            digitalWrite(6, HIGH);
        
        if((adc_temp <= 5) && (adc_temp > 0))
            digitalWrite(5, HIGH);
        
        if((adc_temp <= 10) && (adc_temp > 5))
            digitalWrite(4, HIGH);
        
        if((adc_temp <= 15) && (adc_temp > 10))
            digitalWrite(3, HIGH);
        
        if((adc_temp <= 20) && (adc_temp > 15))
            digitalWrite(2, HIGH);
        
        if((adc_temp < 25) && (adc_temp > 20))
            digitalWrite(1, HIGH);
        
        if(adc_temp >= 25)
            digitalWrite(0, HIGH);
        
        
	}
}

