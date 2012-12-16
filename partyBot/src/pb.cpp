/*
 *  The pb class handles drawing PartyBot (PB) on the screen.  It will receive accelerometer values and based on those, it
 *  will move PB accordingly.
 *
 *  This code is in public domain.  It comes as-is without any guarantees or warranty whatsoever.  You are free to modify and
 *  redistribute it.  If you use this code and like it, you can buy me a beer (or cookies) if we ever meet.
 */

#include "pb.h"


void pb::drawPB(int _x, int _y)
{
    //  This actually draws PB's head, arms and legs
    ofEnableAlphaBlending();
    ofSetColor(0, 0, 255,255);
    ofRect(x - 25, y - 30, 50, 60);     //  PB's body
    ofRect(x - 12, y - 55, 25, 25);     //  PB's head
    ofSetColor(255, 0, 0);
    ofRect(x - 50, y - 20, 25, 7);      //  PB's left arm
    ofRect(x + 25, y - 20, 25, 7);      //  PB's right arm
    ofSetColor(0, 25, 255);
    ofRect(x - 15, y + 30, 7, 25);      //  PB's left foot
    ofRect(x + 10, y + 30, 7, 25);      //  PB's right foot
    ofDisableAlphaBlending();
    
    
    //  Check ADC values and move PB
    
    //  Check for right motion
    if((_x <= posThr) && (_x != -1))
        x -= movPix;
    
    //  Check for down motion
    if((_y <= posThr) && (_y != -1))
        y -= movPix;
    
    //  Check for left motion
    if((_x >= negThr) && (_x != -1))
        x += movPix;
    
    //  Check for down motion
    if((_y >= negThr) && (_y != -1))
        y += movPix;
    
    //  Check to see if PB moves outside of horizontal boundary
    if((x + 50) >= ofGetWidth())
        x = ofGetWidth() - 50;
    
    if(x <= 0)
        x = 0;
    
    //  Check to see if PB moves outside of vertical boundary
    if((y + 60) >= ofGetHeight())
        y = ofGetHeight() - 60;
    
    if(y <= 0)
        y = 0;
    
}

//  This function is for when an arduino is not connected.  You can still control PB using the mouse.
void pb::mouseMove(int _x, int _y)
{
    x = _x;
    y = _y;
    
    ofEnableAlphaBlending();
    ofSetColor(0, 0, 255,255);
    ofRect(x - 25, y - 30, 50, 60);     //  PB's body
    ofRect(x - 12, y - 55, 25, 25);     //  PB's head
    ofSetColor(255, 0, 0);
    ofRect(x - 50, y - 20, 25, 7);      //  PB's left arm
    ofRect(x + 25, y - 20, 25, 7);      //  PB's right arm
    ofSetColor(0, 25, 255);
    ofRect(x - 15, y + 30, 7, 25);
    ofRect(x + 10, y + 30, 7, 25);
    ofDisableAlphaBlending();
}


/*  Partybot's bubble is divided into quadrants.  Each quadrant lights up a different LED.  Quadrants are defined as follows:
    
        2  |  1
       ----|----
        3  |  4
 */
void pb::testQuadrant(int _x, int _y)
{
    /*
    //  Console out for debugging
    cout << "x: " << _x << endl;
    cout << "y: " << _y << endl;
     */
    
    //  Test for collisions in Quadrant 1
    if((_x <= x + radius) && (_x >= x) && (_y >= y - radius) && (_y <= y))
        quad = 1;
    
    //  Test for collisions in Quadrant 2
    if((_x >= x - radius) && (_x < x) && (_y >= y - radius) && (_y <= y))
        quad = 2;
    
    //  Test for collisions in Quadrant 3
    if((_x >= x - radius) && (_x < x) && (_y <= y + radius) && (_y > y))
        quad = 3;
    
    //  Test for collisions in Quadrant 4
    if((_x <= x + radius) && (_x >= x) && (_y <= y + radius) && (_y > y))
        quad = 4;
}

//  This function doesn't light up LEDs.  Instead it displays what LED should be lit up using differently coloured boxes
//  on the top left of the screen.  When a ball hits PB, the information on which quadrant the collision happened must
//  be passed to this function.
void pb::ledIndicate(int _quad)
{
    //  Create differently coloured squares for each LED
    ofNoFill();
    ofSetColor(0, 0, 255);
    ofRect(10, 10, 25, 25);
    ofSetColor(255, 0, 0);
    ofRect(40, 10, 25, 25);
    ofSetColor(0, 255, 0);
    ofRect(70, 10, 25, 25);
    ofSetColor(255, 255, 0);
    ofRect(100, 10, 25, 25);
    
    //  Once the quadrant information is received, the appropriate indicator (box) is coloured.
    switch(_quad)
    {
        case 1:
            ofFill();
            ofSetColor(0, 0, 255);
            ofRect(10, 10, 25, 25);
            break;
            
        case 2:
            ofFill();
            ofSetColor(255, 0, 0);
            ofRect(40, 10, 25, 25);
            break;
            
        case 3:
            ofFill();
            ofSetColor(0, 255, 0);
            ofRect(70, 10, 25, 25);
            break;
            
        case 4:
            ofFill();
            ofSetColor(255, 255, 0);
            ofRect(100, 10, 25, 25);
            break;
            
        default:
            ofNoFill();
            ofSetColor(255, 0, 0);
            ofRect(10, 10, 25, 25);
            ofSetColor(0, 255, 0);
            ofRect(40, 10, 25, 25);
            ofSetColor(0, 0, 255);
            ofRect(70, 10, 25, 25);
            ofSetColor(255, 255, 0);
            ofRect(100, 10, 25, 25);
            
            break;
            
    }
    
    ofFill();
    
}




