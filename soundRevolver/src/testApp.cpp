#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    
    sender.setup(HOST, PORT);
    
    video.setVerbose(true);
    video.setDeviceID(0);
    video.initGrabber(w, h);
    
    colorImg.allocate(w,h);
    blur.allocate(w,h);
    hsv.allocate(w,h);
    hue.allocate(w,h);
    sat.allocate(w,h);
    val.allocate(w,h);
    redFiltered.allocate(w,h);
    blueFiltered.allocate(w,h);

}

//--------------------------------------------------------------
void testApp::update(){
    
    bool bNewFrame = false;
    
    video.update();
    bNewFrame = video.isFrameNew();
    
    if(bNewFrame){
        
        colorImg.setFromPixels(video.getPixels(), w, h);
        colorImg.mirror(false, true);
        
        hsv = colorImg;
        hsv.convertRgbToHsv();
        hsv.convertToGrayscalePlanarImages(hue, sat, val);
        
        //hue.blurGaussian();
        //hue.blurHeavily();
        

        for(int i = 0; i < (w * h); i++){
            
            //redFiltered.getPixels()[i] = ofInRange(hue.getPixels()[i], 160, 170) ? 255 : 0;     //  evening/dark
            redFiltered.getPixels()[i] = ofInRange(hue.getPixels()[i], 25, 35) ? 255 : 0;     //  daylight (default)
            //redFiltered.getPixels()[i] = ofInRange(hue.getPixels()[i], 169, 179) ? 255 : 0;   //  fluorescent lights
            
            blueFiltered.getPixels()[i] = ofInRange(hue.getPixels()[i], 84, 94) ? 255 : 0;
            
            //redFiltered.getPixels()[i] = ofInRange(hue.getPixels()[i], 169, 179) ? 255 : 0;
            //blueFiltered.getPixels()[i] = ofInRange(hue.getPixels()[i], 80, 98) ? 255 : 0;
            
        }
 
        redFiltered.erode_3x3();
        //redFiltered.dilate_3x3();
        //blueFiltered.dilate_3x3();
        blueFiltered.erode_3x3();
        
        redFiltered.flagImageChanged();
        blueFiltered.flagImageChanged();
        
        //redContours.findContours(redFiltered, 200, w*h/2, 2, false);        //  evening/dark
        redContours.findContours(redFiltered, 50, w*h/2, 3, false);       //  daylight (default)
        //redContours.findContours(redFiltered, 500, w*h/2, 2, false);
        //blueContours.findContours(blueFiltered, 500, w*h/2, 3, false);
        
        redBlobs = redContours.nBlobs;
        
        //  Change detecting code needs to go here
        
        if(redBlobs != prev_redBlobs){
            
            if(prev_redBlobs != -1){
                for(int i = 0; i < prev_redBlobs; i++){
                
                    delete rNode[i];
                }
            }
            
           // cout << "NEW OBJECTS CREATED!!" << endl;
        
            rNode = new redNode * [redBlobs];
        
            for(int i = 0; i < redBlobs; i++){
                
                int x, y;
            
                if(debug){
                    x = redContours.blobs[i].centroid.x;
                    y = redContours.blobs[i].centroid.y;
                }
                
                if(!debug) {
                    x = ofMap(redContours.blobs[i].centroid.x, 0, 640, 0, ofGetWidth());
                    y = ofMap(redContours.blobs[i].centroid.y, 0, 480, 0, ofGetHeight());
                }
            
                rNode[i] = new redNode(x,y,i);
            }
            
            prev_redBlobs = redBlobs;
            
        }
        
        for(int i = 0; i < redBlobs; i++){
            
            rNode[i] -> updateTheta();
            
            if(debug){
                rNode[i] -> x = redContours.blobs[i].centroid.x;
                rNode[i] -> y = redContours.blobs[i].centroid.y;
            }
            
            if(!debug) {
                rNode[i] -> x = ofMap(redContours.blobs[i].centroid.x, 0, 640, 0, ofGetWidth());
                rNode[i] -> y = ofMap(redContours.blobs[i].centroid.y, 0, 480, 0, ofGetHeight());
            }
        }

        
    }

}

//--------------------------------------------------------------
void testApp::draw(){

    if(debug) {
        colorImg.draw(0,0);
        redFiltered.draw(650, 0);
        blueFiltered.draw(0, 490);
    }
    
    for(int i = 0; i < redBlobs; i++){
            
        //  Draw all nodes
        rNode[i] -> drawRedNode(blueFiltered, w, h);
        ofSetColor(255, 255, 255);
        ofNoFill();
        //  Send tones to pd patch
        if(rNode[i] -> toneFlag){
            int y_map;
            if(debug){
                y_map = ofMap(rNode[i] -> tone_y, 0, w, 0, 29);
            }
            
            if(!debug) {
                y_map = ofMap(rNode[i] -> tone_y, 0, ofGetHeight(), 29, 0);
            }
            //cout << "y_map= " << y_map << endl;
            ofxOscMessage m;
            m.setAddress("/mouse/position");
            m.addFloatArg(notes[y_map]);
            sender.sendMessage(m);
            
            rNode[i] -> toneFlag = false;
            
        }
    
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    int mx = x & w;
    int my = y % h;
    
    findHue = hue.getPixels()[my*w+mx];
    
    cout << "Hue= " << findHue << endl;
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
