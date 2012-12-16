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


#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
    ofSetWindowTitle("oF-partyBot");
    
	ofBackground(0,0,0);
    
    //  Set number of balls here.  Keep in mind that there are only 30 tones defined.  You can add 100 balls but only 30 will
    //  actually play tones.
    nballs = 31;
    
    //  create ball objects
    ball = new ofBall*[nballs];
    
    for(int i = 0; i < nballs - 1; i++)
    {
        //  Define starting positions for each ball
        float x = 20+(100*i);
        float y = 20+(100*i);
        
        //  Define ball radius
        int dim = 45;
        
        ball[i] = new ofBall(x, y, dim, i, nballs, ball);
    }
    
    //  We create a special bubble for PB to live in
    ball[nballs - 1] = new ofBall(0,0,pb.radius,nballs-1,nballs,ball);
    
    //  Colour PB's bubble white to distinguish it from the others
    ball[nballs - 1]->r = 255;
    ball[nballs - 1]->g = 255;
    ball[nballs - 1]->b = 255;
    
    
    
    
    //  Connect to Arduino.  The firmata protocol requires a baudrate of 57600
    ard.connect("/dev/tty.usbserial-AH013GMK", 57600);
    
    /*  I've tried to use the following lines of code to get the lilypad to send analog values but I always get -1
        for some reason.  I have instead used listeners like the ones in the oF Firmata example
    if(ard.isArduinoReady())
    {
        ard.sendAnalogPinReporting(0, ARD_ANALOG);
        ard.sendAnalogPinReporting(1, ARD_ANALOG);
    }
     */
    
    //  Add a listener so when the Arduino does come online, setupArduino() will be run
    ofAddListener(ard.EInitialized, this, &testApp::setupArduino);
    bsetupArduino = false;
    
    /*
    midi.setup();
    midi.setBank(0, 0x00);
    midi.setVolume(0, 50);
    midi.setInstrument(0, 50);
     */
    

}

//  Call this function when Arduino comes online
void testApp::setupArduino(const int & version)
{
    ofRemoveListener(ard.EInitialized, this, &testApp::setupArduino);
    
    //  Initialize MIDI parameters here
    
    midi.setup();
    midi.setBank(0, 0x00);
    midi.setVolume(0, 50);
    midi.setInstrument(0, 50);
    
    bsetupArduino = true;
    
    //  Enable analog and digital pins on the Lilypad
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
    ard.sendAnalogPinReporting(1, ARD_ANALOG);
    
    ard.sendDigitalPinMode(10, ARD_OUTPUT);
    ard.sendDigitalPinMode(5, ARD_OUTPUT);
    ard.sendDigitalPinMode(4, ARD_OUTPUT);
    ard.sendDigitalPinMode(2, ARD_OUTPUT);
    
}


void testApp::update(){
    
    
    ard.update();
    spd_sum = 0;

    //  Get ADC values from accelerometer
    ax = ard.getAnalog(0);
    ay = ard.getAnalog(1);
  
    for(int i = 0; i < nballs - 1; i++)
    {
        ball[i] -> update();
        ball[i] -> collide();
        if(ball[i] -> collFlag)
        {
            pb.testQuadrant((int)(ball[i] ->x), (int)(ball[i] -> y));
            
            if(ball[i]->quadrant != pb.quad)
            {   
                //ack = midi.acknowledge();
                //midi.midiDrain();
                //if(ack == 1)
                //{
                    midi.playNote(0, noteBank[bank][i]);
                    //midi.midiDrain();
                    ball[i] ->quadrant = pb.quad;
                //}
                
                //cout << "No ACK Received" << endl;
            }

        }
        
        if(ball[i] -> collFlag == false)
            ball[i] -> quadrant = 0;
        
        ball[i] -> collFlag = false;
        
        
    }
    
    ball[nballs-1] -> x = pb.x;
    ball[nballs-1] -> y = pb.y;
    
    ard.sendDigital(10, 0);
    ard.sendDigital(5, 0);
    ard.sendDigital(4, 0);
    ard.sendDigital(2, 0);
    
    //  Calculate the average speed of all the balls
    for(int i = 0; i < nballs-1; i++)
    {
        spd_sum += sqrt(((ball[i] -> speedX)*(ball[i] -> speedX)) + ((ball[i] -> speedY)*(ball[i] -> speedY)));
    }
    
    avg_spd = spd_sum / (nballs - 1);
    

}

//--------------------------------------------------------------
void testApp::draw(){
    

    
    stringstream report;
    stringstream start_msg;
    stringstream friction;
    
    ofSetColor(255, 255, 255);
    
    if(!hide)
    {
        start_msg << "'h' = Display Status + Options" << endl;
        ofDrawBitmapString(start_msg.str(), 10, 840);
    }
    
    
    //  LEDs are lit up here.  It might be better off in the pb class but oh well.
    switch (pb.quad) {
        case 1:
            ard.sendDigital(10, 1);
            break;
            
        case 2:
            ard.sendDigital(5, 1);
            break;
            
        case 3:
            ard.sendDigital(4, 1);
            break;
            
        case 4:
            ard.sendDigital(2, 1);
            break;
            
        default:
            break;
    }
    
    //  Draw balls here
    for(int i = 0; i < nballs; i++)
    {
        ball[i] -> draw();
        
    }
    
    //  If PB is not connected, control PB using a mouse
    if(bsetupArduino == false)
    {
        pb.mouseMove(mx,my);
    }
    
    else
    {
        pb.drawPB(ax,ay);
    }
    
    if(hide)
    {
        ofSetColor(255, 255, 255);
    
        report << "FPS: " << ofGetFrameRate() << endl << "analog X: " << ax << endl << "analog Y: " << ay << endl << "X: " << pb.x << endl << "Y: " << pb.y << endl << "Friction: " << ball[0]->friction << endl << "Quadrant: " << pb.quad << endl
        << "Instrument: " << instruments[instr_counter] << endl << "Max Speed: " << ball[0] -> maxSpeed << endl << "Average Speed: " << avg_spd << endl;
        
        if(!fric)
            friction << "Friction Mode Disabled" << endl;

        
        if(fric)
            friction << "Friction Mode Enabled" << endl;
            
        start_msg << "Options:" << endl << endl << "'i' = Change Instrument" << endl << "'t' = Change Music Scale" << endl << "'c' = Center partyBot" << endl << "'f' = Enable/Disable Friction Mode" << endl << "'h' = Hide Status + Options" << endl << "'+' = Increase Speed" << endl << "'-' = Decrease Speed" << endl;
    
        ofDrawBitmapString(report.str(), 10, 652);
        ofDrawBitmapString(friction.str(), 10, 800);
        ofDrawBitmapString(start_msg.str(), 10, 100);
        pb.ledIndicate(pb.quad);
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    //  Change friction values for every ball
    switch(key)
    {
        case '+':
        case '=':
            
            for(int i = 0; i < nballs; i++)
            {
                ball[i]->maxSpeed += 10;
            }
            break;
            
        case '-':
        case '_':
            
            for(int i = 0; i <nballs; i++)
            {
                
                ball[i]->maxSpeed -= 10;
                
                if(ball[i] -> maxSpeed <= 0)
                    ball[i] -> maxSpeed = 0;
            }
            break;
        
        //  Press f to enable or disable friction
        case 'f':
            
            if(!fric)
            {
                for(int i = 0; i <nballs; i++)
                {
                    ball[i] -> friction = -0.05;
                }
                
                fric = true;
            }
            
            else if(fric)
            {
                for(int i = 0; i < nballs; i++)
                {
                    ball[i] -> friction = 0;
                }
                
                fric = false;
            }
            
            break;
            
        //  Press c to center PB on the screen
        case 'c':

            pb.x = ofGetWidth() / 2;
            pb.y = ofGetHeight() / 2;
            
            break;
        
        //  Press h to hide or show the status stuff
        case 'h':
            
            if(hide == true)
                hide = false;
            
            else if(hide == false)
                hide = true;
            
            break;
        
        //  Press t to change scale
        case 't':
            
            bank++;
            
            if(bank > 1)
                bank = 0;
            
            break;
        
        //  Press i to change instrument
        case 'i':
            
            instr_counter++;
            
            if(instr_counter > 7)
                instr_counter = 0;
            
            midi.setInstrument(0, instr[instr_counter]);
            
            break;

            
    }


}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
    //  Update the variables mx & my with current mouse position
    if(bsetupArduino == false)
    {
        mx = x;
        my = y;

        
    }
     

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    bank++;
    
    if(bank > 1)
        bank = 0;
    

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}