/*
 *  The pb class handles drawing PartyBot (PB) on the screen.  It will receive accelerometer values and based on those, it
 *  will move PB accordingly.
 *
 *  This code is in public domain.  It comes as-is without any guarantees or warranty whatsoever.  You are free to modify and
 *  redistribute it.  If you use this code and like it, you can buy me a beer (or cookies) if we ever meet.
 */

#ifndef __partyBot__pb__
#define __partyBot__pb__

#include <iostream>
#include "ofMain.h"

class pb
{
    
public:
    
    //  Function called in testApp::draw to draw PB
    void drawPB(int _x, int _y);
    //  Checks where a collision happened
    void testQuadrant(int _x, int _y);
    //  Fills a square that indicates which LED is supposed to light up on PB
    void ledIndicate(int _quad);
    //  If PB is not connected, you can still move PB around with a mouse
    void mouseMove(int _x, int _y);
    
    //  Starting position for PB is the center of the window
    int x = ofGetWidth() / 2;
    int y = ofGetHeight() / 2;
    
    
    //  PB's bubble radius
    int radius = 100;
    
    int quad = 0;
    
    //  Define threshold accelerometer ADC values.  If ADC value is above posThr, PB will move up or right
    //  If ADC value is above negThr, PB will move down or left
    int posThr = 450;
    int negThr = 530;
    
    //  Move PB by this many pixels
    int movPix = 15;

    
    
};

#endif /* defined(__partyBot__pb__) */
