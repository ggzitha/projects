/*
  This is a small sketch that relays serial communication from one device
  at a certain baud rate and outputs the same byte at a different baud rate
  
  This is based off of the Serial Event sketch by Tom Igoe.
  
  Created by Allen Lee (alee@meoworkshop.org)
  
  Rev. O: 4-Nov/12
  
*/

#include <SoftwareSerial.h>

#define BAUD_IN 9600    //  Define a baudrate for incoming serial signal
#define BAUD_OUT 31250  //  Define a baudrate for outgoing serial signal


String inputString = "";
boolean stringComplete = false;
char input = 0;

//  Set up software serial for outgoing serial signals.  RX/TX 2/3
SoftwareSerial serialOut(2,3);

void setup()
{
  Serial.begin(BAUD_IN);
  serialOut.begin(BAUD_OUT);
  inputString.reserve(1);    //  Reserve 1 byte for the string
  //Serial.print('e');
  
  pinMode(4, OUTPUT);
  //digitalWrite(4, HIGH);
  digitalWrite(4, LOW);
  //delay(50);
  digitalWrite(4,HIGH);
}


void loop()
{
  if(stringComplete)
  {
    
    //Serial.println(input);
    serialOut.print(input);
    inputString = "";
    stringComplete = false;
    
    //Serial.print("e");
  }

  
}

void serialEvent()
{
  while(Serial.available())
  {
    char inChar = (char)Serial.read();
    //inputString += inChar;
    input = inChar;
    
    //if(inChar == '\n')
      stringComplete = true;
  }
}
  
