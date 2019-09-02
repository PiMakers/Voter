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
        ofTrueTypeFont::setGlobalDpi(72);
        rWindow.setup();
//        datawindow.setup();
//        rWindow.voteMode = "Titkos";
}

//--------------------------------------------------------------
void ofApp::update() {

        width = ofGetWindowWidth()/3;
        height = ofGetWindowHeight()/15;
        
        if (arsReciver -> connected) {



                
            if (arsReciver -> vote_started ) {
                        if (arsReciver -> vote_ended)
                                rWindow.countDown -> ended = true;
                                
                        if ( !rWindow.countDown -> isStarted() ) {

                                rWindow.countDown -> start();
                        }

            }
            
            else  if ( rWindow.countDown -> isStarted() && rWindow.countDown -> ended) {
                        arsReciver -> resetVoteResult();
                        rWindow.countDown -> stop();
                }
                
            if (rWindow.countDown -> stopped) {
                        
                        //rWindow.setCounterType ( arsReciver -> stopWatch_button );
                        
                        rWindow.voteMode = "Nyílt";
                        if ( arsReciver -> graf_button)
                            rWindow.voteMode = "Titkos";

                        if (( arsReciver -> udArrow || arsReciver -> lrArrow) && (!rWindow.countDown -> counterType )) {
                    
                            rWindow.setStartTime (10000*(arsReciver -> udArrow) + 60000*(arsReciver -> lrArrow) + DEFAULT_VOTE_TIME);
                            ofLogVerbose(__func__) << "arsReciver -> lrArrow" << arsReciver -> lrArrow;
                            ofLogVerbose(__func__) << "arsReciver -> udArrow" << arsReciver -> udArrow;
                        }
            }
            
        }
        rWindow.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

    rWindow.draw();

    if (arsReciver -> connected) {

        if (!arsReciver -> vote_started) {
            if (arsReciver -> udArrow || arsReciver -> lrArrow) {
                
                //if ( !rWindow.countDown -> isStarted() )
                    rWindow.setStartTime (10000*(arsReciver -> udArrow) + 60000*(arsReciver -> lrArrow) + DEFAULT_VOTE_TIME);
                
                //ofLogVerbose(__func__) << "rWindow.countDown -> startTime" << rWindow.countDown -> startTime;
            }
        }

            //if (!rWindow.countDown -> ended)
                rWindow.numOfYes = 0;
                rWindow.numOfNo = 0;
                rWindow.numOfElse = 0;
            rWindow.numOfNotVoted = 0;
            rWindow.numOfAway = MAX_NUM_VOTERS;

        for (size_t i = 0; i < arsReciver -> votes.size(); i++) {

        	color = ofGetBackgroundColor();

            if ( arsReciver -> votes[i].registered ) {
            	rWindow.numOfAway--;
            }

            //if ( arsReciver -> vote_started ){
            if ( rWindow.countDown -> isStarted()) {
                if ( arsReciver -> votes[i].vote_value == 1 ) {
                    	rWindow.numOfYes++;
                    if ( rWindow.countDown -> ended && (rWindow.voteMode.compare("Nyílt") == 0))
                        	color = ofColor::green;
                }
                else if ( arsReciver -> votes[i].vote_value == 2 ) {
                    rWindow.numOfNo++;
                    if ( rWindow.countDown -> ended && (rWindow.voteMode.compare("Nyílt") == 0))
                        color = ofColor::red;
                }
                else if ( arsReciver -> votes[i].vote_value == 3 ) {
                    rWindow.numOfElse++;
                    if ( rWindow.countDown -> ended && (rWindow.voteMode.compare("Nyílt") == 0))
                        color = ofColor::white;
                }
            }    

            if (rWindow.voteMode.compare("Nyílt") == 0) {
                        
                posX = (int)(arsReciver -> votes[i].index )/10; // tizesek -> sor
                posY = arsReciver -> votes[i].index-10*posX; // egyesek -> oszlop pld: 29 = 9.sor 2.oszlop

                posX = posX * width;
                posY = (5 + posY)*height;

                ofFill();
                ofSetColor (color);
                ofDrawRectangle (posX, posY, width, height );

                // black border
                ofNoFill();
                ofSetColor(ofColor::black);
                ofDrawRectangle (posX, posY, width, height );

                if ( ! arsReciver -> votes[i].registered ) {
                    ofSetColor ( 0, 100);
                    boundingBox = rWindow.ttf30i.getStringBoundingBox(arsReciver -> votes[i].voter_name, 0,0);
                    rWindow.ttf30i.drawString( arsReciver -> votes[i].voter_name, posX + 0.5*(width-boundingBox.width), posY + height-0.5*(height- boundingBox.height));
                }
                else {
                    boundingBox = rWindow.ttf30.getStringBoundingBox(arsReciver -> votes[i].voter_name, 0,0);
                    rWindow.ttf30.drawString( arsReciver -> votes[i].voter_name, posX + 0.5*(width-boundingBox.width), posY + height-0.5*(height- boundingBox.height));
                }
            }
            else {
                ofNoFill();
                //ofEnableAlphaBlending();
                //                ofSetColor(ofColor::green,127);
                ofSetColor(0, 255,0, 50);
                ofDrawRectangle ( 1*ofGetWidth()/4-75 , 0.9*ofGetHeight(), 150, ofGetHeight()/-2);
                ofSetColor(ofColor::red,50);
                ofDrawRectangle ( 2*ofGetWidth()/4-75, 0.9*ofGetHeight(), 150, ofGetHeight()/-2);
                ofSetColor(ofColor::white,50);
                ofDrawRectangle ( 3*ofGetWidth()/4-75, 0.9*ofGetHeight(), 150, ofGetHeight()/-2);
                //ofDisableAlphaBlending();
                /*
                ofLogVerbose (__func__) << "rWindow.numOfElse++;     " << rWindow.numOfElse;
                ofLogVerbose(__func__) << "rWindow.numOfYes++;     " << rWindow.numOfYes;
                ofLogVerbose(__func__) << "rWindow.numOfNo++;     " << rWindow.numOfNo;
                */
                if ( rWindow.countDown -> ended ) {
                    ofFill();
                    ofSetColor(ofColor::green, 100);
                    ofDrawRectangle ( 1*ofGetWidth()/4-75 , 0.9*ofGetHeight(), 150, rWindow.numOfYes*ofGetHeight()/-60);
                    ofSetColor(ofColor::red);
                    ofDrawRectangle ( 2*ofGetWidth()/4-75, 0.9*ofGetHeight(), 150, rWindow.numOfNo*ofGetHeight()/-60);
                    ofSetColor(ofColor::white);
                    ofDrawRectangle ( 3*ofGetWidth()/4-75, 0.9*ofGetHeight(), 150, rWindow.numOfElse*ofGetHeight()/-60);
                    // ofDrawRectangle ( 3*ofGetWidth()/4-75, 0.9*ofGetHeight(), 150, 30*ofGetHeight()/-60);
                }
            }
        }
    }
//    arsReciver -> drawCounter(rWindow.counterFont, 200,200);
//    datawindow.drawCirculeGraaf();
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
        if ( arsReciver ) {
                delete arsReciver;
                arsReciver = NULL;
        }
}