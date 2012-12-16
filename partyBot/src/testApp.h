/*  partyBot 
 *
 *  Created by:  Allen Lee (alee@meoworkshop.org)
 *  Revision:  rev.O 16-Dec/12
 *
 *  More information about this project can be found at :  http://meoworkshop.org/projects/partybot
 *
 *  This code is in public domain.  It comes as-is without any guarantees or warranty whatsoever.  You are free to modify and
 *  redistribute it.  If you use this code and like it, you can buy me a beer (or cookies) if we ever meet.
 */


#pragma once

#include "ofMain.h"
#include "pb.h"
#include "ofBall.h"
#include "midi.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    bool bsetupArduino;
    bool hide = false;
    bool fric = true;
    int ax, ay;
    int spd_sum = 0;
    int avg_spd;
    
    int mx;
    int my;
    
    //  Number of balls
    int nballs;
    //  Indicates which scale is used.  There are two so far which you can see in char noteBank[][]
    int bank = 1;
    //  Instruments available.  There are more which you can find in the VS1053B datasheet
    int instr[8] = {0, 6, 14, 45, 46, 55, 47, 104};
    string instruments[8] = {"Piano", "Harpsichord", "Bells", "Pizz Strings", "Harp", "Orchestral Hit", "Timpani", "Sitar"};
    //  Indicates which instrument is used
    int instr_counter = 0;
    //  Bank of notes to play.  Each row is a different scale.
    char noteBank[2][30] = {{37,39,42,44,46,49,51,54,56,58,61,63,66,68,70,73,75,78,80,82,85,87,90,92,94,97,99,102,104,106},
        { 33,36,38,41,45,48,50,53,57,60,62,65,69,72,74,77,81,84,86,89,93,96,98,101,105,108}};
    //  I used this for debugging.  You can ignore this.
    int ack = 0;

    
    ofArduino ard;
    pb pb;
    midi midi;
    ofBall** ball;
    
private:
    
    void setupArduino(const int & version);
    
    
};
