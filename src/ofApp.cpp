//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"

//#define VENDOR_ID 1008
//#define PRODUCT_ID 34836

//--------------------------------------------------------------
void ofApp::setup()
{
        gui.setup();
}

//--------------------------------------------------------------
void ofApp::update() {
        gui.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
/*
        if (arsReciver.connected)
            arsReciver.draw();
*/
        gui.draw();            
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key)
    {
        case 'f' :
                ofToggleFullscreen();
                break;
        
        case OF_KEY_F1:
                ofSetLogLevel ( OF_LOG_VERBOSE );
                break;
            
        case OF_KEY_F2:
                ofSetLogLevel ( OF_LOG_NOTICE );
                break;

        case OF_KEY_F3:
                ofSetLogLevel ( OF_LOG_ERROR );
                break;
            
        case OF_KEY_F4:
                ofSetLogLevel ( OF_LOG_SILENT );
                break;        
            
            default:
                break;
    }

//    ofLogError("Loglevel: ", ofGetLogLevel );
}

//--------------------------------------------------------------
void ofApp::exit() {

}