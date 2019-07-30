#include "resultWindow.h"

resultWindow::resultWindow(){
        ///        arsReciver = new ARS_reciver;
        };

resultWindow::~resultWindow(){};

void resultWindow::setup(float scale) {
	ofTrueTypeFont::setGlobalDpi(72);

        ofTrueTypeFontSettings settings20("verdana.ttf", scale*25);
        settings20.addRange(ofUnicode::Latin);
        settings20.addRange(ofUnicode::Latin1Supplement);
        settings20.addRange(ofUnicode::LatinA);
        ttf20.load(settings20);

        ofTrueTypeFontSettings settings30("verdana.ttf", scale*30);
        settings30.addRange(ofUnicode::Latin);
        settings30.addRange(ofUnicode::Latin1Supplement);
        settings30.addRange(ofUnicode::LatinA);
        ttf30.load(settings30);

        ofTrueTypeFontSettings settings30i("verdanaItalic.ttf", scale*30);
        settings30i.addRange(ofUnicode::Latin);
        settings30i.addRange(ofUnicode::Latin1Supplement);
        settings30i.addRange(ofUnicode::LatinA);
        ttf30i.load(settings30i);        

        ofTrueTypeFontSettings settings40("verdana.ttf", scale*40);
        settings40.addRange(ofUnicode::Latin);
        settings40.addRange(ofUnicode::Latin1Supplement);
        settings40.addRange(ofUnicode::LatinA);
        ttf40.load(settings40);

        counterFont.load("verdana.ttf", scale*90);
        
        //countDown -> counterType(STOP_WATCH);
        }
void resultWindow::update() {
        
        height = ofGetHeight()/15;
        width = ( ofGetWidth() - 5*height ) / 2;
    }

void resultWindow::draw( float x, float y, float scaleW, float scaleH) {
        //Draw titleBar & Titles
        ofSetColor(ofColor::lightBlue);
        ofFill();
        ofDrawRectangle ( x, y, ofGetWidth() - x, ofGetHeight()/15);

        ofSetColor(ofColor::white);
        title = voteMode + " szavazás";
        mainRect = ttf30.getStringBoundingBox(title, 0,0);
        ttf30.drawString( title , x + (width - mainRect.width)/2 , (mainRect.height + ofGetWindowHeight()/15)/2 );
        title = "Statisztika:";
        mainRect = ttf30.getStringBoundingBox(title, 0,0);
        ttf30.drawString( title , width + (width - mainRect.width)/2 , (mainRect.height + ofGetWindowHeight()/15)/2 );
        mainRect = ttf30.getStringBoundingBox("Idő:", 0, 0);
        ttf30.drawString( "Idő:" , 2* width + (5*height - mainRect.width)/2 , (mainRect.height + ofGetWindowHeight()/15)/2 );

        //Draw MainFrame
        ofSetColor(ofColor::black);
        mainRect = ttf20.getStringBoundingBox( ofGetTimestampString( "%Y.%m.%d " ) + translateDayName(), 0, 0);
        ttf20.drawString( ofGetTimestampString( "%Y.%m.%d " ) + translateDayName() , 2* width + (5*height - mainRect.width)/2 , (mainRect.height + ofGetWindowHeight()/15)/0.8 );
        mainRect = ttf20.getStringBoundingBox(ofGetTimestampString( "%H:%M:%S" ), 0, 0);
        ttf20.drawString( ofGetTimestampString( "%H:%M:%S" ) , 2* width + (5*height - mainRect.width)/2 , (mainRect.height + ofGetWindowHeight()/15)/0.55 );

        ofNoFill();
        ofDrawRectangle ( x, 0, width, 5*height);
        ofDrawRectangle ( width, 0, width, 5*height);
        ofDrawRectangle ( 2* width, 0, 5*height, 5*height);


        // Draw value text
        if (countDown -> isStarted() ) {
                mainRect = ttf20.getStringBoundingBox("ÉREM SZAVAZZON!", 0, 0);
                std::string tmpString = "KÉREM SZAVAZZON!";
                if (countDown -> ended) {
                        tmpString = " SZAVAZÁS VÉGE! ";
                        ofSetColor(ofColor::red);
                }
                        ////////////////////
                        ttf20.drawString( tmpString , 2* width + (5*height - mainRect.width)/2 , (mainRect.height + ofGetWindowHeight()/15)/0.42 );                
                        ofSetColor(ofColor::black);
        }

        mainRect = ttf30.getStringBoundingBox("Idő:", 0, 0);
        stringBoundingBox = ttf40.getStringBoundingBox(ofToString(numOfYes), 0,0);

        ttf40.drawString( "Igen:", 0.08 * width,  2*height + stringBoundingBox.height/2 );
        ttf40.drawString( "Nem:", 0.08 * width,  3*height + stringBoundingBox.height/2 );
        ttf40.drawString( "Tartózkodik:", 0.08 * width, 4*height + stringBoundingBox.height/2 );
        ttf40.drawString( "Szavazott:", 1.08 * width,  2*height + stringBoundingBox.height/2 );
        ttf40.drawString( "Nem szavazott:", 1.08 * width,  3*height + stringBoundingBox.height/2 );
        ttf40.drawString( "Távol (Nem regisztrált):", 1.08 * width, 4*height + stringBoundingBox.height/2 );

        if ( countDown -> ended ) {
                ttf40.drawString( ofToString(numOfYes), 0.92 * width - stringBoundingBox.width, 2*height + stringBoundingBox.height/2 );

                ttf40.drawString( ofToString(numOfNo), 0.92 * width - stringBoundingBox.width, 3*height + stringBoundingBox.height/2 );

                ttf40.drawString( ofToString(numOfElse), 0.92 * width - stringBoundingBox.width, 4*height + stringBoundingBox.height/2 );
        
                numOfVoted = numOfYes + numOfNo + numOfElse;
                ttf40.drawString( ofToString(numOfVoted), 1.92 * width - stringBoundingBox.width, 2*height + stringBoundingBox.height/2 );

                numOfNotVoted = max ( MAX_NUM_VOTERS - numOfVoted - numOfAway, 0 );
                ttf40.drawString( ofToString(numOfNotVoted), 1.92 * width - stringBoundingBox.width, 3*height + stringBoundingBox.height/2 );
        }
        
        ttf40.drawString( ofToString(numOfAway), 1.92 * width - stringBoundingBox.width, 4*height + stringBoundingBox.height/2 );
        
        mainRect = counterFont.getStringBoundingBox("00:00", 0,0);
        countDown -> draw( counterFont, ofGetWindowWidth() - 2.5*height - mainRect.width/2, 4*height + mainRect.height/2 );

    }

void resultWindow::setCounterType ( bool stopWatch_button ) {
        countDown -> setCounterType ( (int) stopWatch_button );
        }

void resultWindow::setStartTime ( int newStartTime ) {
        countDown -> setStartTime (  newStartTime );
        }
        
string resultWindow::translateDayName() {

            switch(ofGetWeekday()) {

                case 1:
                        return "Hétfő";
                case 2:
                        return "Kedd";
                case 3:
                        return "Szerda";
                case 4:
                        return "Csütörtök";
                case 5:
                        return "Péntek";
                case 6:
                        return "Szombat";
                case 7:
                        return "Vasárnap";
                default :
                        return "UnknownDay";
        }
    }

