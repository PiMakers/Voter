#pragma once

#include "ofMain.h"
#include "resultWindow.h"

#define MAIN_WINDOW_HEIGHT 3

class GUI : public ofBaseApp
{

public:
    GUI (/* args */){};
    ~GUI(){} ;

    void setup();
    void update();
    void draw();

    //resultWindow
    


    ofTrueTypeFont ttf10, ttf15, ttf20;
    std::string str;
    typedef struct _INFO_WINDOW {

        std::string title;
//        ofRectangle mainWindow = ofRectangle( 0, 0, ofGetWindowWidth(), ofGetWindowHeight()/MAIN_WINDOW_HEIGHT);

    } INFO_WINDOW;

    INFO_WINDOW iWindow;


private:

    /// ofRectangle bBox;
    resultWindow rWindow;
};
/*
GUI : public ofBaseApp::GUI : public ofBaseApp( args )
{
}

GUI : public ofBaseApp::~GUI : public ofBaseApp()
{
}
*/