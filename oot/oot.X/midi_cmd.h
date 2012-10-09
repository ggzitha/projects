/* 
 * This is a very very basic MIDI library.  This library can be used with any
 * MIDI IC without too much problems.  I have used the Sparkfun Music Instrument
 * Shield using the VS1053B IC with this library and found no problems...yet.
 *
 * Only one channel is used!
 *
 * This library is under the BEERWARE License.  If you like it, you can buy me a
 * beer if we ever meet.  I also accept cookies.
 *
 * Documentation is found at http://meoworkshop.org/doc/midi-lib
 *
 * Written by Allen Lee
 * alee@meoworkshop.org
 */

#ifndef MIDI_CMD_H
#define	MIDI_CMD_H

    void midi_init(void);
    void setBank(char channel, char bank);
    void setVol(char channel, char vol);
    void setInstrument(char channel, char instr);
    void playNote(char channel, char note);
    void stopNote(char channel, char note);
    void txMIDI(char cmd, char data1, char data2);
    void setTempo(float tempo);
    void nr(char type);

#endif	/* MIDI_CMD_H */

