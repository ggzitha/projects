/*
 *  The MIDI Class is responsible for sending MIDI commands to the Music Instrument Shield
 *  I've based this upon the example code written by Nathan Seidle at Sparkfun.
 *  The original was written for the arduino and can be found here: http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Dev/Arduino/Shields/MusicIntrument_Example.pde
 *
 *  This class communicates to a uC over serial at 9600 baud.  Firmware in the uC must be written to compliment this class.
 *
 *  This code is in public domain.  It comes as-is without any guarantees or warranty whatsoever.  You are free to modify and
 *  redistribute it.  If you use this code and like it, you can buy me a beer (or cookies) if we ever meet.
 */

#include "midi.h"

void midi::setup()
{
    //  Open serial connection
    serial.setup("/dev/tty.usbmodem1421",9600);
}

//  This function does the actual transmitting.
void midi::txmidi(char cmd, char data1, char data2)
{
    serial.writeByte(cmd);
    serial.writeByte(data1);
    
    
    if((cmd & 0xf0) <= 0xb0)
    {
        serial.writeByte(data2);
    }
}

//  Function to change bank of instruments on the VS1053b
void midi::setBank(char channel, char bank)
{
    txmidi((ctrl_chg | channel), 0x00, bank);
}

//  Function to set Volume up to 127
void midi::setVolume(char channel, char vol)
{
    txmidi((ctrl_chg | channel), vol_chg, vol);
}

//  Function to set Instrument.  Refer to the VS1053B Datasheet for a list of instruments available
void midi::setInstrument(char channel, char instr)
{
    txmidi(((char)0xc0 | channel),instr,(char)0x00);
}

//  Functions to play and top a note.  Note that some instruments like the piano don't need a "stopNote()" to complement a playNote()
//  instruments like the piano will stop automatically.  Instruments like the flute do not so it will require a stopNote()
void midi::playNote(char channel, char note)
{
    txmidi((play | channel), note, attack);
}

void midi::stopNote(char channel, char note)
{
    txmidi((stop | channel), note, release);
}

//  I wrote this for debugging purposes since I was running into problems with synchronization.  You can ignore this.
int midi::acknowledge()
{
    ack = serial.readByte();
    
    if(ack == 'e')
    {
        cout << "ACK Received.  Send moar data plz" << endl;
        return 1;
    }
    
    return 0;
}

//  This was also for debugging
void midi::midiDrain()
{
    //serial.drain();
    serial.flush();
}


