#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "redNode.h"

#define HOST "localhost"
#define PORT 12345

class testApp : public ofBaseApp{

	public:
    
    bool debug = false;
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        int w = 640;
        int h = 480;
    int screen_w = ofGetWidth();
    int screen_h = ofGetHeight();
    
        int findHue;
    int findVal;
    
    int redBlobs;
    int blueBlobs;
    int prev_redBlobs = -1;
    
    char notes[30] = {37,39,42,44,46,49,51,54,56,58,61,63,66,68,70,73,75,78,80,82,85,87,90,92,94,97,99,102,104,106};
    
    
    ofVideoGrabber video;
    ofxCvColorImage colorImg, hsv;
    
    ofxCvGrayscaleImage hue, sat, val, blur;
    ofxCvGrayscaleImage redFiltered, grnFiltered, blueFiltered;
    ofxCvContourFinder redContours, blueContours;
    
    ofxOscSender sender;
    
    redNode ** rNode;

    

};

