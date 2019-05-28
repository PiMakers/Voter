#include "GUI.h"


void GUI::setup() {
	ofTrueTypeFont::setGlobalDpi(72);

	ttf10.load("verdana.ttf", 10, true, true);
	ttf10.setLineHeight(18.0f);
	ttf10.setLetterSpacing(1.037);

    ttf15.load("verdana.ttf", 15, true, true);
	ttf15.setLineHeight(18.0f);
	ttf15.setLetterSpacing(1.037);

	ttf20.load("verdana.ttf", 20, true, true);
	ttf20.setLineHeight(18.0f);
	ttf20.setLetterSpacing(1.037);

    rWindow.setup();

}

void GUI::update(){
    rWindow.update();
}

void GUI::draw() {
/*
    //INFO WINDOW
    ofSetColor(225);
    ofFill();
    ofDrawRectangle( 0, 0, ofGetWindowWidth(), ofGetWindowHeight()/MAIN_WINDOW_HEIGHT);
    
    //INFO WINDOW TEXT (szavazás eredménye)
    ofSetColor(ofColor::black);
    str = "Nyílt szavazás";
    ofRectangle rect = ttf.getStringBoundingBox(str, 0,0);

    ofDrawRectangle(rect.x, rect.y, rect.width, rect.height);
    ttf.drawString(str, 5, 10);
    */
//	ttf10.drawString(str/* + ofToString(ttf.getSize())*/, 0, ofGetWindowHeight()/MAIN_WINDOW_HEIGHT);
//    ofNoFill();
///     ofDrawRectangle( 0, 0, ofGetWindowWidth()*3/7, ofGetWindowHeight()/MAIN_WINDOW_HEIGHT);

    rWindow.draw();
    rWindow.draw( ofGetWindowWidth()*3/7, 0);

    //INFO WINDOW TEXT (szavazás statisztika szavazott, nem szavazott , távol)
}