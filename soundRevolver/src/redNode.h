//
//  node.h
//  cvtest
//
//  Created by superkittens on 2013-03-23.
//
//

#ifndef __cvtest__node__
#define __cvtest__node__

#include <iostream>
#include <vector.h>
#include <math.h>

#include "ofMain.h"
#include "ofxOpenCv.h"

class redNode{
    
public:
    
    bool debug = false;
    
    int x;
    int y;
    int jitter_thresh = 15;
    int index;
    int min_dist = 350;
    
    int width;
    int height;
    
    int x_prev = -1;
    int y_prev = -1;
    
    //  Variables for an orbiting circle
    float theta = 0;
    int counter = 0;
    double velocity = ofRandom(0.1,0.3);
    const float pi = 3.14;
    int x_orbit;
    int y_orbit;
    int orbit_radius = 50;

    
    bool toneFlag = false;
    bool noteFired = false;
    int tone_y;
    
    ofxCvContourFinder blueChildContour;
    
    redNode(int, int, int);
    
    void findChild(ofxCvGrayscaleImage, int w, int h);
    void updateTheta();
    void drawRedNode(ofxCvGrayscaleImage, int w, int h);
};

#endif /* defined(__cvtest__node__) */
