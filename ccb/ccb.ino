
/*  Coffee Cup Bells project using the pile of coffee cups "graciously donated" by the Java Connection coffeeshop
 There are 5 cups which emit a tone when someone smacks it. 
 
 The MIDI portion of the code works off of the efforts of Sparkfun's Nathan Seidle and Rick Winscot.
 Rick's VS1053 Code is available here: https://gist.github.com/1492432
 
 
 By Allen Lee [alee@meoworkshop.org]
 Detailed project notes: http://meoworkshop.org/projects
 Code is in public domain (Beerware License)
 
 Instruments that sound good: 0, 9 & 47
 
 */

#include <SoftwareSerial.h>

#define RST 4    // Reset Pin

//  Connect to VS1053 TX/RX 3/2
SoftwareSerial midi(1,3);

//  Set up parameters for MIDI
byte sound_bank = 0x00;
int vol = 127;
int atk = 60;
int rel = 80;
int del = 50;    //  Delay between play and stops

byte play = 0x90;
byte stp = 0x80;

int instrument = 0;

//  Assign ports for inputs and LED outputs as well as MIDI Tone Numbers
//  Index array is set to 0x80 or "STOP" by default until a cup is hit
static int cup[5] = {2,5,6,7,8};              //  Assign input ports here
int cup_tone[5] = {57,60,62,65,69};           //  Tones for each cup
byte index[5] = {0x80,0x80,0x80,0x80,0x80};   //  This is where hits (play/stop info) for each cup are stored

static int light_pin[5] = {9,10,11,12,13};    //  Assign output ports here
int light[5] = {0,0,0,0,0};                   //  This is where hits (ON/OFF info) for each cup are stored.


unsigned long int timer = 0;  //  Set a variable for millis()
boolean mark = 0;             //  Indicates if any bell has been hit
int threshold = 80;           //  Timer Limit


void setup()
{  
  //  Set up cup inputs
  pinMode(cup[0], INPUT);
  pinMode(cup[1], INPUT);
  pinMode(cup[2], INPUT);
  pinMode(cup[3], INPUT);
  pinMode(cup[4], INPUT);

  //  Set up light outputs
  pinMode(light_pin[0], OUTPUT);
  pinMode(light_pin[1], OUTPUT);
  pinMode(light_pin[2], OUTPUT);
  pinMode(light_pin[3], OUTPUT);
  pinMode(light_pin[4], OUTPUT);

  pinMode(RST, OUTPUT);
  
  //  Reset VS1053
  digitalWrite(RST,LOW);
  delay(100);
  digitalWrite(RST, HIGH);
  delay(100);
  

  //  Set up MIDI parameters
  midi.begin(31250);    // MIDI is always 31250
  setBank(sound_bank);
  setVolume(vol);
  setInstrument(instrument);
  
  //  Flash all cups one at a time to indicate ready status
  for(int i=0; i<4; i++)
  {
    digitalWrite(light_pin[i], HIGH);
    delay(50);
    
    digitalWrite(light_pin[i],LOW);
    delay(50);
  }
  
}

void loop()
{

  timer = millis();    //  Update timer variable
  while(1)
  { 
    //  Continuously loop until timer has expired
    //  for loop cycles through each cup testing if it has been hit or not
    for(int i = 0; i < 5; i++)
    {
      if(digitalRead(cup[i]))
      {
        index[i] = 0x90;
        light[i] = 1;

        mark = 1;
      }
    }

    if((millis() - timer) > threshold)
    {

      //  Instead of going to the genTone function all the time, program goes to function only if a cup has been hit - indicated by "mark"
      if(mark)
      {
        genTone(index,light);
        
        //  Reset Indices once function is finished
        index = {0x80,0x80,0x80,0x80,0x80};
        light = {0,0,0,0,0};
      }

      mark = 0;
      break;
    }
  }


}

//  setBank, setVolume, setInstrument & setNote have been borrowed from Rick's MIDI Code

void setBank(byte bank)
{
  midi.write(0xB0);
  midi.write((byte)0);
  midi.write(bank);
}

void setVolume(int vol)
{
  midi.write(0xB0);
  midi.write(0x07);
  midi.write((byte)vol);
}

void setInstrument(byte inst)
{
  midi.write(0xC0);
  midi.write(inst);
}


void setNote(byte note, byte cmd)
{
  midi.write(cmd);
  midi.write(note);
  midi.write((cmd == play ? atk : rel));
}


//  Tone generation and LED lighting happens here.  
void genTone(byte index[], int light[])
{
  //  Play tones if 0x90 has been assigned, do nothing otherwise
  setNote(cup_tone[0], index[0]);
  setNote(cup_tone[1], index[1]);
  setNote(cup_tone[2], index[2]);
  setNote(cup_tone[3], index[3]);
  setNote(cup_tone[4], index[4]);

  //  Light applicable LEDs
  digitalWrite(light_pin[0], light[0]);
  digitalWrite(light_pin[1], light[1]);
  digitalWrite(light_pin[2], light[2]);
  digitalWrite(light_pin[3], light[3]);
  digitalWrite(light_pin[4], light[4]);

  delay(del);
  
  //  Stop playback for all cups
  setNote(cup_tone[0], stp);
  setNote(cup_tone[1], stp);
  setNote(cup_tone[2], stp);
  setNote(cup_tone[3], stp);
  setNote(cup_tone[4], stp);

  //  Turn off all LEDs
  digitalWrite(light_pin[0], LOW);
  digitalWrite(light_pin[1], LOW);
  digitalWrite(light_pin[2], LOW);
  digitalWrite(light_pin[3], LOW);
  digitalWrite(light_pin[4], LOW);
  delay(del);

}



