#pragma once

#include "ofMain.h"


class dataWindow  {

public:
 
    void setup (float endAngle = 90){
        
        // red
        origo = {ofGetWindowWidth() / 2, ofGetWindowHeight() / 2};
        curve.arc(origo, 300, 300, 0, endAngle);
	    curve.close();
	    curve.setArcResolution(60);  
        curve.setFillColor(red);  
	    curve.setFilled(true);
//        curve.moveTo(100,200);
        
        ofSetColor(ofColor::blue);
        curve.arc(origo, 300, 300, 100, 120);
//        curve.setFillColor(green);
//        curve.setFilled(true); 

    };
 
    void drawCirculeGraaf() {
        curve.draw();
    }

private:

    ofPath curve;  
	ofPoint origo;
    ofColor red = ofColor::red;
    ofColor green = ofColor::green;
    ofColor white = ofColor::white;;

};