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


#include "ofBall.h"


//  Constructor
//  The user can explicitly define each parameter except for _bid and object oin.  The object, oin is in fact, an object of
//  ofBall (yo dawg...).  An object is instantiated within its own class to allow collision functionality.
ofBall::ofBall(float _x, float _y, int _dim, int _bid, int _nBalls, ofBall** oin)
{
	x = _x;
	y = _y;
	dim = _dim;
    bid = _bid;
    nBalls = _nBalls;
    
    others = oin;
	
	speedX = ofRandom(-maxSpeed, maxSpeed);
	speedY = ofRandom(-maxSpeed, maxSpeed);

    
	
}


//  Here, the speed of a ball is changed around.  Speed can change from different collisions but there is a speed limit defined.
void ofBall::update(){
    
    //  Set speed limits or else the balls will go too fast!
    if((speedX > maxSpeed) || (speedX < -maxSpeed))
        speedX = ofRandom(-10,10);
    
    if((speedY > maxSpeed) || (speedY < -maxSpeed))
        speedY = ofRandom(-10,10);
	
	if((x-dim) < 0 ){
        x = dim;
        speedX *= -1;
	}
    
    //  If the ball hits the window boundary, these if-else statements allow the ball to bounce away from the boundary
    else if((x+dim) > ofGetWidth()){
        x = ofGetWidth() - dim;
        speedX *= -1;
	}
	
	
	if((y-dim) < 0 ){
        y = dim;
        speedY *= -1;
	}
    
    else if((y+dim) > ofGetHeight()){
        y = ofGetHeight() - dim;
        speedY *= -1;
	}
	
    //  The speed of a ball is updated here.  When friction is disabled (from pressing the 'f' key), friction = 0.
    speedX = speedX + (speedX*friction);
    speedY = speedY + (speedY*friction);
    
	x += speedX;
	y += speedY;
	
}

//  This function is called in testApp::draw()
void ofBall::draw(){
	
    ofEnableAlphaBlending();
	ofSetColor(r,g,b,127);
	ofCircle(x, y, dim);
    ofFill();
    ofDisableAlphaBlending();
    
}

/*  The collision portion has been ported from the Processing example found at http://processing.org/learning/topics/bouncybubbles.html .  Of course, there is no gravity here like in the Processing example.
 
    I'm not sure if I understand this completely myself but it works.
 */

void ofBall::collide()
{
    for(int i = bid + 1; i < nBalls; i++)
    {
        //  Find the distance between two balls
        float dx = others[i]->x - x;
        float dy = others[i]->y - y;
        float distance = sqrt((dx*dx) + (dy*dy));
        
        //  The minimum distance (ie. two balls just touching each other) is the sum of its two diameters
        float minDist = (others[i]->dim) + (dim);
        
        //  If the distance between the balls is less than the minimum distance, start the collision calculations
        if(distance < minDist)
        {
            //  "collFlag" is a flag that is raised when a collision between a ball and PB's bubble occurs.  This is used for playing tones and determining the quadrant where the collision happened
            if(others[i]->x == others[nBalls - 1]->x)
                collFlag = true;
            
            float angle = atan2(dy, dx);
            float targetX = x + (cos(angle) * minDist);
            float targetY = y + (sin(angle) * minDist);
            
            float ax = (targetX - others[i]->x) * spring;
            float ay = (targetY - others[i]->y) * spring;
            
            speedX -= ax;
            speedY -= ay;
            
            others[i]->speedX += ax;
            others[i]->speedY += ay;
        }
    }
    
}



