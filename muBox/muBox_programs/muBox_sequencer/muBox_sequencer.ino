/*  meoWS muBox Firmware:  Sequencer
 *  Use the potentiometer to choose the number of notes.
 *  Next, use the potentiometer to choose the notes to played back in sequence.  Press button 1 to register the note.
 *  To reset the sequence, press button 2
 *  Once the last note is registered, the sequence will play
 *  On playback, use the potentiometer to adjust speed
 *  To stop and reset, press button 2
 *  
 *  
 *  The following pins are used:
 *    D2 - Button
 *    D3 - Button
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
 *  rev.O [16-Nov/12]
 */

//  mubox requires Software Serial to communicate MIDI
#include <SoftwareSerial.h>


//  Define Pins as explained above
#define BU1 2
#define BU2 3
#define SDI 4
#define CLK 5
#define LE 6
#define SEL A0
#define RST 7

//  Create Softserial object for midi
SoftwareSerial midi(9,8);

int numNotes = 16;    //  Set maximum number of notes playable
int seq[16];          //  Set up an array that stores user-defined notes.  Note that the maximum array size is 16
int aVal = 0;         //  Variable for potentiometer reads
int aVal_temp = 0;    //  Temporary variable for pot reads.  You'll see how it is used later on
int light[16] ={
  0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000,0x8000};    //  Define the 16-bit strings for lighting arrays

//  The notes to select are hard coded in this bank here.  You may change it whatever you wish
char noteBank[16] = {
  48,51,55,58,60,63,67,70,72,75,79,82,84,87,91,94};    //  Define selectable notes.  You can change this!

void setup()
{
  //  Setup buttons.  You can set up interrupts.  Do so carefully!
  //attachInterrupt(0, <fcn>, FALLING);
  //attachInterrupt(1, <fcn>, FALLING);
  pinMode(BU1, INPUT);
  pinMode(BU2, INPUT);
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
  setInstrument(0,0);    //  You can change this to a different instrument if you wish.  BE CAREFUL!  For pretty much most instruments, you will also need to use the stopNote() function or else a note will play forever!
  
  //  Reset the VS1053b Chip
  digitalWrite(RST, LOW);
  delay(50);
  digitalWrite(RST, HIGH);

  //  Serial for debugging
  Serial.begin(9600);

}

//  Not a lot goes on in the loop as you can see.  There are three functions.  The first is the set the number of notes in a sequence, the next is a function for selecting the notes to play in sequence and the last
//  actually plays the sequence.
void loop()
{
  Serial.println("setNumber()");
  setNumber();
  Serial.print("numNotes= ");
  Serial.println(numNotes,DEC);
  Serial.println("setNotes()");
  setNotes();
  Serial.println("Playing Sequence");
  playSeq();  

  Serial.println("RESET");
}

//  Define function to set the number of notes in a sequence
void setNumber(void)
{
  //  We want an infinite loop here until the user is finished selecting the number of notes!
  while(1)
  {
    aVal_temp = analogRead(SEL);              //  We don't have 1024 LEDs so we first put the analogRead in a temporary variable
    aVal = map(aVal_temp, 0, 1023, 0, 15);    //  The temp variable is then mapped from 0 to 15.  aVal stores the mapped value

    lightLED(light[aVal]);                    //  Light the LED!

    //  Check to see if the user has selected the number of notes.
    //  A button press pulls the pin LOW
    if(digitalRead(BU1) == 0)
    {
      //  Flash the LEDs to confirm that the number has been set
      for(int i = 0; i < 2; i++)
      {
        lightLED(light[0]);
        delay(100);
        lightLED(0x0000);
        delay(100);
      }
      //  Debounce delay
      delay(200);
      numNotes = aVal + 1;                    //  The number of notes is stored here!  
      break;                                  //  Break out of the infinite loop and back to the loop() area
    }
  }
}


//  Set what notes we want the sequencer to play here    
void setNotes(void)
{ 
  int counter = 0;                            //  Counter keeps track of how many notes you have selected.  Once counter reaches the max number of notes the user has defined, it will break out of the function
  int aVal_prev = 0;                          //  Used to prevent the VS1053 from repeatly playing the same note if pot is untouched
  while(1)
  {
    aVal_temp = analogRead(SEL);              //  Again, we have mapping
    aVal = map(aVal_temp, 0, 1023, 0, 15);

    lightLED(light[aVal]);

    //  We only want the note to play once!
    if(aVal_prev != aVal)
    {
      playNote(0, noteBank[aVal]);
      aVal_prev = aVal;
    }
    
    //  Check to see if user has selected a note
    if(digitalRead(BU1) == 0)
    {
      //  Flash the LED to confirm that the note has been put into the sequence array
      for(int i=0; i < 2; i++)
      {
        lightLED(light[aVal]);
        delay(100);
        lightLED(0x0000);
        delay(100);
      }
      
      delay(200);
      seq[counter] = noteBank[aVal];
      counter++;                                //  Increment counter

      //  Check to see if maximum number of user-defined notes has been reached
      if(counter == numNotes)
      {
        //  Flash all LEDs to indicate the sequence is about to play
        for(int i=0; i < 2; i++)
        {
          lightLED(0xffff);
          delay(100);
          lightLED(0x0000);
          delay(100);
        }

        break;
      }
      
      
    }

    //  Check to see if the user pressed the button to redo his selections
    if(digitalRead(BU2) == 0)
    {
      delay(200);
      counter = 0;
    }
  }
}


//  Here, we actually play back the sequence
void playSeq(void)
{
  while(1)
  {
    for(int i = 0; i < numNotes; i++)
    {
      aVal = analogRead(SEL);

      playNote(0,seq[i]);
      lightLED(light[i]);
      delay(aVal);
    }
    
    //  Check to see if user has pressed reset to start the whole process again
    if(digitalRead(BU2) == 0)
    {
      delay(200);
      break;
    }
  }
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


