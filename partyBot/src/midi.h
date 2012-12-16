/*
 *  The MIDI Class is responsible for sending MIDI commands to the Music Instrument Shield
 *  I've based this upon the example code written by Nathan Seidle at Sparkfun.
 *  The original was written for the arduino and can be found here: http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Dev/Arduino/Shields/MusicIntrument_Example.pde
 *
 *  This class communicates to a uC over serial at 9600 baud.  Firmware in the uC must be written to compliment this class.
 *  Why 9600 and not 31250??  Well, I woud love to connect directly to the shield, but 31250 baud isn't actually defined
 *  in oFSerial which is why a uC acting as a "relay" is used.
 *
 *  This code is in public domain.  It comes as-is without any guarantees or warranty whatsoever.  You are free to modify and
 *  redistribute it.  If you use this code and like it, you can buy me a beer (or cookies) if we ever meet.
 */


#include "ofMain.h"

#ifndef __midi_test__midi__
#define __midi_test__midi__

class midi
{
private:
    //  Attack and release values.  You can change these to suit your liking
    char attack = 60;
    char release = 80;
    //  These are command bytes.  DON'T CHANGE THESE!!
    char play = 0x90;
    char stop = 0x80;
    char bank_sel = 0x00;
    char ctrl_chg = 0xb0;
    char vol_chg = 0x07;
    
    //  Used for debugging.  You can ignore it.
    char ack;
    
    
public:
    
    //  Opens a serial connection
    void setup();
    //  Sends user specified commands
    void txmidi(char cmd, char data1, char data2);
    void setBank(char channel, char bank);
    void setVolume(char channel, char vol);
    void setInstrument(char channel, char instr);
    void playNote(char channel, char note);
    void stopNote(char channel, char note);
    
    //  The following functions are used for debugging.
    int acknowledge(void);
    void midiDrain(void);
    
    //  Create an object from oFSerial
    ofSerial serial;
    
    
};


#endif /* defined(__midi_test__midi__) */
