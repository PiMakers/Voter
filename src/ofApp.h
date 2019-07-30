//
// Copyright (c) 2010 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ARS_reciver.h"
#include "resultWindow.h"
// #include "dataWindow.h"

class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
	
	void keyPressed(int key);
	void windowResized(int w, int h);

	void exit();
/*	
    void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
*/

//    ARS_reciver arsReciver;
//	GUI gui;

	resultWindow rWindow;
    ARS_reciver* arsReciver = new ARS_reciver;
    counter* cnt = new counter;


    float width, height;
    float posX, posY, index = -1;

    ofTrueTypeFont vote_font;
    ofRectangle boundingBox;
    ofColor color;
	float scale, scaleW, scaleH;
//	dataWindow datawindow;

};
