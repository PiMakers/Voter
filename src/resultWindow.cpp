#include "resultWindow.h"

resultWindow::resultWindow(){
///        arsReciver = new ARS_reciver;
};

resultWindow::~resultWindow(){

};

void resultWindow::setup() {
	ofTrueTypeFont::setGlobalDpi(72);

        ofTrueTypeFontSettings settings20("verdana.ttf", 25);
        settings20.addRange(ofUnicode::Latin);
        settings20.addRange(ofUnicode::Latin1Supplement);
        settings20.addRange(ofUnicode::LatinA);
        ttf20.load(settings20);

        ofTrueTypeFontSettings settings30("verdana.ttf", 30);
        settings30.addRange(ofUnicode::Latin);
        settings30.addRange(ofUnicode::Latin1Supplement);
        settings30.addRange(ofUnicode::LatinA);
        ttf30.load(settings30);

        ofTrueTypeFontSettings settings30i("verdanaItalic.ttf", 30);
        settings30i.addRange(ofUnicode::Latin);
        settings30i.addRange(ofUnicode::Latin1Supplement);
        settings30i.addRange(ofUnicode::LatinA);
        ttf30i.load(settings30i);        

        ofTrueTypeFontSettings settings40("verdana.ttf", 40);
        settings40.addRange(ofUnicode::Latin);
        settings40.addRange(ofUnicode::Latin1Supplement);
        settings40.addRange(ofUnicode::LatinA);
        ttf40.load(settings40);

        counterFont.load("verdana.ttf", 90);
        
//        countDown -> counterType(STOP_WATCH);
}

void resultWindow::update() {
//        width = ofGetWindowWidth()*3/7;
        height = ofGetWindowHeight()*5/15;
        width = ( ofGetWindowWidth() - height ) / 2;

//        cout << "mainRect" << mainRect.width << endl;
    }

void resultWindow::setCounterType ( bool stopWatch_button ) {
        countDown -> setCounterType ( (int) stopWatch_button );
}

void resultWindow::setStartTime ( int newStartTime ) {
        countDown -> setStartTime (  newStartTime );
}

void resultWindow::draw( float x, float y) {
        //Draw titleBar
        ofSetColor(ofColor::lightBlue);
        ofFill();
        title1 = voteMode + " szavazás";
        mainRect = ttf20.getStringBoundingBox(title1, 0,0);
        ofDrawRectangle ( x, y, ofGetWindowWidth() - x, ofGetWindowHeight()/15);
//        mainRect = ttf20.getStringBoundingBox(title2, 0,0);
//        ofDrawRectangle ( width, Y, width, mainRect.height + 20);

        //Draw title
        ofSetColor(ofColor::white);
 //       ttf20.drawString( title2 , width + (width - mainRect.width)/2 , ofGetWindowHeight()/30 );
        mainRect = ttf30.getStringBoundingBox(title1, 0,0);
        ttf30.drawString( title1 , x + (width - mainRect.width)/2 , (mainRect.height + ofGetWindowHeight()/15)/2 );
        mainRect = ttf30.getStringBoundingBox(title2, 0,0);
        ttf30.drawString( title2 , width + (width - mainRect.width)/2 , (mainRect.height + ofGetWindowHeight()/15)/2 );
        
        mainRect = ttf30.getStringBoundingBox("Idő:", 0, 0);
        ttf30.drawString( "Idő:" , 2* width + (height - mainRect.width)/2 , (mainRect.height + ofGetWindowHeight()/15)/2 );

        //Draw MainFrame
        ofSetColor(ofColor::black);
        
        mainRect = ttf20.getStringBoundingBox(ofGetTimestampString( "%Y.%m.%d %A" ), 0, 0);
        ttf20.drawString( ofGetTimestampString( "%Y.%m.%d %A" ) , 2* width + (height - mainRect.width)/2 , (mainRect.height + ofGetWindowHeight()/15)/0.8 );
        mainRect = ttf20.getStringBoundingBox(ofGetTimestampString( "%H:%M:%S" ), 0, 0);
        ttf20.drawString( ofGetTimestampString( "%H:%M:%S" ) , 2* width + (height - mainRect.width)/2 , (mainRect.height + ofGetWindowHeight()/15)/0.5 );

        ofNoFill();
        ofDrawRectangle ( x, 0, width, height);
        ofDrawRectangle ( width, 0, width, height);
        ofDrawRectangle ( 2* width, 0, height, height);


        // Draw value text
        ofSetColor(ofColor::black);
        if (countDown -> isStarted() ) {
                mainRect = ttf20.getStringBoundingBox("ÉREM SZAVAZZON!", 0, 0);
                std::string tmpString =          "KÉREM SZAVAZZON!";
                if (countDown -> ended) {
                        tmpString = " SZAVAZÁS VÉGE! ";
                        ofSetColor(ofColor::red);
                }
                        ttf20.drawString( tmpString , 2* width + (height - mainRect.width)/2 , (mainRect.height + ofGetWindowHeight()/15)/0.42 );                
                        ofSetColor(ofColor::black);
        }

        mainRect = ttf30.getStringBoundingBox("Idő:", 0, 0);
        stringBoundingBox = ttf40.getStringBoundingBox(ofToString(numOfYes), 0,0);

        ttf40.drawString( "Igen:", 0.08 * width,  4 * mainRect.height + 15 );
        ttf40.drawString( "Nem:", 0.08 * width,  8 * mainRect.height + 15 );
        ttf40.drawString( "Tartózkodik:", 0.08 * width, 12 * mainRect.height + 15 );
        ttf40.drawString( "Szavazott:", 1.08 * width,  4 * mainRect.height + 15 );
        ttf40.drawString( "Nem szavazott:", 1.08 * width,  8 * mainRect.height + 15 );
        ttf40.drawString( "Távol (Nem regisztrált):", 1.08 * width, 12 * mainRect.height + 15 );

        if ( countDown -> ended ) {
                ttf40.drawString( ofToString(numOfYes), 0.92 * width - stringBoundingBox.width, 4 * mainRect.height + 15 );

                ttf40.drawString( ofToString(numOfNo), 0.92 * width - stringBoundingBox.width, 8 * mainRect.height + 15 );

                ttf40.drawString( ofToString(numOfElse), 0.92 * width - stringBoundingBox.width, 12 * mainRect.height + 15 );
        }
                numOfVoted = numOfYes + numOfNo + numOfElse;
                ttf40.drawString( ofToString(numOfVoted), 1.92 * width - stringBoundingBox.width, 4 * mainRect.height + 15 );

                numOfNotVoted = MAX_NUM_VOTERS - numOfVoted - numOfAway;
                ttf40.drawString( ofToString(numOfNotVoted), 1.92 * width - stringBoundingBox.width, 8 * mainRect.height + 15 );

                ttf40.drawString( ofToString(numOfAway), 1.92 * width - stringBoundingBox.width, 12 * mainRect.height + 15 );
                /*
                ofLogVerbose (__func__) << "rWindow.numOfElse++;     " << numOfElse;
                ofLogVerbose(__func__) << "rWindow.numOfYes++;     " << numOfYes;
                ofLogVerbose(__func__) << "rWindow.numOfNo++;     " << numOfNo;
                */
        
        countDown -> draw( counterFont, ofGetWindowWidth() - 0.85 * height, 12 * mainRect.height + 15 );

    }
