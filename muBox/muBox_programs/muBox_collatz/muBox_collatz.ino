/*  meoWS Collatz Box Firmware
 *  This program uses the Collatz Conjecture to play music.  If a resulting number is even, then a regular note from a predefined scale is played.
 *  If a resulting number is odd, the chord progression changes.
 *
 *  The following pins are used:
 *    D2 - Start Button
 *    D3 - Change Instrument Button
 *    D4 - SDI (LED Driver)
 *    D5 - CLK (LED Driver)
 *    D6 - LE (LED Drier)
 *    D7 - VS1053b Reset
 *    D8 - VS1053b MIDI in
 *    D9 - VS1053b RX (Not used)
 *    A0 - Number Selector (Pot)
 *
 *  Developed by:  Allen Lee (alee@meoworkshop.org)
 *  Revision History
 *  rev.O [12-Oct/12]
 */

//  mubox requires Software Serial to communicate MIDI
#include <SoftwareSerial.h>

//  Define Pins as explained above
#define START 2
#define CHG 3
#define SDI 4
#define CLK 5
#define LE 6
#define SEL A0
#define RST 7

SoftwareSerial midi(9,8);

int aVal = 0;    //  Value for the analog input
int scaled_aval = 0;

//  Define a bunch of notes to play
int notes[3][12] =  {
  {
    65,67,69,72,74,77,79,81,84,86,89,91  }
  ,
  {
    69,72,74,77,79,81,84,86,89,91,93,96  }
  ,
  {
    72,74,77,79,81,84,86,89,91,93,96,98  }
};

int del = 80;    //  Define a note duration 

volatile int instrument[9] = {0,13,31,45,46,47,52,79,94};
volatile int index = 0;



void setup()
{
  //  Setup buttons & their associated interrupts
  //attachInterrupt(0, start, FALLING);
  attachInterrupt(1, chg, FALLING);
  pinMode(START, INPUT);
  pinMode(CHG, INPUT);
  pinMode(RST, OUTPUT);

  //  Setup LED Driver pins
  pinMode(SDI, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(LE, OUTPUT);
  //  Byte is latched in the LED Driver when LE is active low
  digitalWrite(LE, HIGH);

  //  Set up MIDI Parameters
  midi.begin(31250);
  setBank(0,0x00);
  setVol(0,100);
  setInstrument(0,0);

  //  Reset the VS1053b Chip
  digitalWrite(RST, LOW);
  delay(50);
  digitalWrite(RST, HIGH);

  //  Serial for debugging
  Serial.begin(9600);

}


void loop()
{
  //setInstrument(0,0);
  
  aVal = analogRead(SEL);
  scaled_aval = map(aVal,0,1023,0,131071);
  lightLED(scaled_aval);  
  
  if(digitalRead(START))
  {
    Serial.println("Start");
    collatz(aVal);
  }

}

//  Interrupt function to change instruments
void chg(void)
{
  if(index > 8)
    index = 0;
  index++;
  setInstrument(0,instrument[index]);
}

//  This function lights up the LEDs for a given value
void lightLED(int value)
{
  digitalWrite(LE, LOW);
  //  Because our LED Driver is a 16 channel one, and SPI takes in a byte at a time, we must split our value to two bytes.  
  //  We achieve sending the higher byte by right shifting our value by one byte (8 bits)
  shiftOut(SDI, CLK, MSBFIRST, (value >> 8));
  shiftOut(SDI, CLK, MSBFIRST, value);
  digitalWrite(LE, HIGH);
}

//  The actual execution of the collatz conjecture for a given number is done here.
void collatz(long int num)
{
  Serial.println("collatz started");
  int counter1 = 0;
  int counter2 = 0;

  while(num != 1)
  {

    //  Even test
    if((num % 2) == 0)
    {
      num = num / 2;
    }

    //  Odd test
    else if((num % 2) == 1)
    {
      Serial.println("ODD");
      num = (num * 3) + 1;
      counter1++;
      
      //  If we are already on the third chord progression, go back to progression 1
      if(counter1 == 3)
      {
        counter1 = 0;
      }
      
      //  Reset the index for regular notes in the scale
      counter2 = 0;
    }

    lightLED((int)num);

    Serial.println(counter1,DEC);
    Serial.println(counter2,DEC);
    playNote(0,notes[counter1][counter2]);
    delay(del);
    stopNote(0,notes[counter1][counter2]);
    
    //  Increment to play the next note in the scale
    counter2++;
  }
}


//  Define some MIDI Functions in this section
void MIDI(char cmd, char data1, char data2)
{
  midi.print(cmd);
  midi.print(data1);

  //  If a command is less than 0xb0, then two data bytes are needed instead of one 
  if((cmd & 0xf0) < 0xb0)
    midi.print(data2);
}

void setBank(char channel, char bank)
{
  MIDI((0xb0 | channel), 0x00, bank);
}

void setVol(char channel, char vol)
{
  MIDI((0xb0 | channel), 0x07, vol);
}

void setInstrument(char channel, char instr)
{
  MIDI((0xc0 | channel), instr, 0);
}

void playNote(char channel, char note)
{
  //  I've preset the attack (and release) value because they're not overly changed around in this case.  If you wish to change the value
  //  you may do so up to 127.
    char atk = 60;
  MIDI((0x90 | channel), note, atk);
}

void stopNote(char channel, char note)
{
  char rel = 80;
  MIDI((0x80 | channel), note, rel);
}








