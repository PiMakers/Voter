#include "resultWindow.h"

resultWindow::resultWindow(){
        arsReciver = new ARS_reciver;
};

resultWindow::~resultWindow(){
        if ( arsReciver ) {
                delete arsReciver;
                arsReciver = NULL;
        }
};

void resultWindow::setup() {
	ofTrueTypeFont::setGlobalDpi(72);

	ttf20.load("verdana.ttf", 20, true, true);
	ttf20.setLineHeight(18.0f);
	ttf20.setLetterSpacing(1.037);


	ttf30.load("verdana.ttf", 30, true, true);
	ttf30.setLineHeight(18.0f);
	ttf30.setLetterSpacing(1.037);

        ttf40.load("verdana.ttf", 40, true, true);
	ttf40.setLineHeight(18.0f);
	ttf40.setLetterSpacing(1.037);


}
void resultWindow::update() {
        width = ofGetWindowWidth()*3/7;
        height = ofGetWindowHeight()*4/15;
        mainRect = ttf20.getStringBoundingBox(title, 0,0);
//        cout << "mainRect" << mainRect.width << endl;
    }

void resultWindow::draw( float x, float Y) {
        //Draw titleBar
        ofSetColor(ofColor::lightBlue);
        ofFill();
        ofDrawRectangle ( x, Y, width, mainRect.height + 20);

        //Draw title
        ofSetColor(ofColor::white);
        ttf20.drawString( title, x + (width - mainRect.width)/2 , mainRect.height + 10 );


        // Draw value text
        ofSetColor(ofColor::black);
        ttf40.drawString( "Igen:                                 19", 0.08 * width,  4 * mainRect.height + 15 );
        ttf40.drawString( "Szavazott:                            19", 1.08 * width,  4 * mainRect.height + 15 );
        ttf40.drawString( "Nem:                                   0", 0.08 * width,  8 * mainRect.height + 15 );
        ttf40.drawString( "Nem szavazott:                      19", 1.08 * width,  8 * mainRect.height + 15 );
        ttf40.drawString( "Tartózkodik:                        19", 0.08 * width, 12 * mainRect.height + 15 );
        ttf40.drawString( "Távol:                              19", 1.08 * width, 12 * mainRect.height + 15 );


        //Draw MainFrame

        ofNoFill();
        ofDrawRectangle ( x, 0, width, height);

        if (arsReciver -> connected)
            arsReciver -> draw();        

    }
