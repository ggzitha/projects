/*  The ofBall class is responsible for creating the balls which collide against PartyBot (PB).
    Also included in this class is collision and "physics" functions. 
    ofBall is not intended to actually simulate physics but the collisions are good enough.  Newton would not be proud.
 
    The collision functionality is ported from the Processing "Bouncy Bubbles" example found at:
    http://processing.org/learning/topics/bouncybubbles.html
    The main differences are that the code here is written in C++ (not Java) and there is no gravity.
 
    The ofBall is derived from the openFrameworks ball example.
 
    The bubble in which PB lives in is actually an object of this class!  It is given special attributes.  The last ball that
    is instantiated from this class is reserved for PB's bubble.
 
 *  This code is in public domain.  It comes as-is without any guarantees or warranty whatsoever.  You are free to modify and
 *  redistribute it.  If you use this code and like it, you can buy me a beer (or cookies) if we ever meet.
 */


#ifndef _OF_BALL	
#define _OF_BALL		


#include "ofMain.h"


class ofBall {
    
    
    
public:
    
    //  Speed and boundary collisions are enforced here
    void update();
    //  This function is called by testApp::draw() and draws a ball
    void draw();
    //  This function handles all collisions between balls
    void collide();
    
    //  Constructor
    ofBall(float x, float y, int dim, int bid, int nBalls, ofBall** oin);
    
    //  x,y position of ball
    float x;
    float y;
    //  Ball diameter
    int dim;
    //  Ball speed (in pixels)
    float speedX;
    float speedY;
    //  When two balls collide, they do not bounce off each other like billiard balls, instead there is some spring
    //  in the collision
    float spring = 0.3;
    //  Set a value for friction
    float friction = -0.05;
    
    int quadrant = 0;
    
    bool collFlag = false;
    //  Assign a random colour for a ball
    int r = ofRandom(0,255);
    int g = ofRandom(0,255);
    int b = ofRandom(0,255);
    
    //  An identifying number given to the ball.  This is determined at testApp::setup()
    int bid;
    int nBalls;
    //  Maximum speed limit for a ball
    int maxSpeed = 40;
    //  "Inception" is happening here.  We are instantiating an object of this class.  This is for the collision function.
    ofBall** others;
    
    
private:
    
    
};					

#endif