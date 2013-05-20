//
//  node.cpp
//  cvtest
//
//  Created by superkittens on 2013-03-23.
//
//

#include "redNode.h"

redNode::redNode(int _x, int _y, int _index){
    
    x = _x;
    y = _y;
    
    if(x_prev == -1){
        x_prev = x;
    }
    
    if(y_prev == -1){
        y_prev = y;
    }
    index = _index;
}

void redNode::findChild(ofxCvGrayscaleImage child, int w, int h){
    
    //  Find Children
    blueChildContour.findContours(child, 500, w*h/2, 5, false);
}

void redNode::updateTheta(){
    
    theta = (velocity * counter);
    counter++;
    
    if(theta >= (2 * pi)){
        theta = 0;
        counter = 0;
    }
    
    x_orbit = (orbit_radius * cos(theta)) + x;
    y_orbit = (orbit_radius * sin(theta)) + y;
}

void redNode::drawRedNode(ofxCvGrayscaleImage child, int w, int h){
    
    //  Find Children
    blueChildContour.findContours(child, 150, w*h/2, 6, false);
    
    width = w;
    height = h;
    
    //  Draw orbiting circle and ring
    ofSetColor(255, 0, 0);
    ofNoFill();
    ofCircle(x, y, orbit_radius);
    ofFill();
    ofCircle(x_orbit, y_orbit, 10);
    ofCircle(x, y, 20);
    
    
    for(int i=0; i < blueChildContour.nBlobs; i++){
        
        int blue_x;
        int blue_y;
        
        if(debug){
            blue_x = blueChildContour.blobs[i].centroid.x;
            blue_y = blueChildContour.blobs[i].centroid.y;
        }
        
        if(!debug){
            blue_x = ofMap(blueChildContour.blobs[i].centroid.x, 0, 640, 0, ofGetWidth());
            blue_y = ofMap(blueChildContour.blobs[i].centroid.y, 0, 480, 0, ofGetHeight());
        }
        
        ofSetColor(127, 127, 127);
        ofSetLineWidth(3);
        
        if(sqrt((blue_x - x) * (blue_x - x) + ((blue_y - y) * (blue_y - y))) <= min_dist){
            
            ofLine(x, y, blue_x, blue_y);

            float childTestTheta = atan2(((float)blue_y - (float)y) , ((float)blue_x - (float)x));
            float childTheta;
        
            // cout << "Theta= " << theta << endl;
        
        
            if(childTestTheta < 0){
                childTheta = (2*pi) + childTestTheta;
            }
        
            else{
                childTheta = childTestTheta;
            }

            //cout << "Child " << i << "= " << childTheta << endl;
            if((theta < (childTheta + 0.25)) && (theta > (childTheta - 0.25))){
                if(!noteFired){
                    ofSetColor(255, 255, 255);
                    ofLine(x, y, blue_x, blue_y);
                    //   cout << "MATCH!" << endl;
            
                    tone_y = blue_y;
                    toneFlag = true;
                    noteFired = true;
                }
            }
            
            noteFired = false;
        
            //  Draw blue nodes
            ofSetColor(0, 0, 255);
            ofNoFill();
            ofCircle(blue_x, blue_y, 35);
            ofFill();
            ofCircle(blue_x, blue_y, 20);

        }
    }
    
}
