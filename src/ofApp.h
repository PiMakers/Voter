//
// Copyright (c) 2010 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
// #include "ofxHID.h"
#include "ARS_reciver.h"
#include "GUI.h"

class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw() override;

	void keyPressed(int key);
/*	
    void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
*/
    void exit() override;

//    ARS_reciver arsReciver;
	GUI gui;

};
