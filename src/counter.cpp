#include "counter.h"


void counter::setCounterType ( int ct ) {
    if (stopped) {
        if ( ct == COUNT_DOWN ) {
            //startTime = DEFAULT_VOTE_TIME;
            endTime = 0;
            i = -1;
            counterType = COUNT_DOWN;
        }
        else if ( ct == STOP_WATCH ) {
            startTime = 0;
            i = 1;
            counterType = STOP_WATCH;
        }
        deltaTime = startTime;
        ended = false;
    }
    else ofLogError(__func__) << "Cannot set setCounterType while running";
}

void counter::setStartTime ( int newStartTime ){
    if (stopped) {
        if (newStartTime < 0) newStartTime = 0;
            startTime = newStartTime;
            deltaTime = startTime;
    }
    else ofLogError(__func__) << "Cannot set startTime while running";
}

bool counter::isStarted() {
        return started;
}

void counter::start() {

    if ( !started || ended) {
        if ( counterType == COUNT_DOWN ) {
        // startTime = DEFAULT_VOTE_TIME;
            endTime = 0;
            i = -1;
        }
        else if ( counterType == STOP_WATCH ) {
            startTime = 0;
            i = 1;
        }

        //        currTime = ofGetElapsedTimeMillis();    
        lastTime = ofGetElapsedTimeMillis();
        started = true;
        ended = false;
        stopped = false;
    }
}

void counter::stop() {
        if ( counterType == COUNT_DOWN ) {
            deltaTime = startTime;
            endTime = 0;
            i = -1;
            started = false;
            ended = true;
        }
        else if ( counterType == STOP_WATCH ) {
            startTime = 0;
//            endTime = DEFAULT_VOTE_TIME;
            i=1;
        }    
//        if (ended) 
        started = false;
        ended = true;
        stopped = true;
//        deltaTime = startTime;
        cout << "stopped!!!!!!!!!!!!!" << endl;
}

int counter::millisToMinutes( uint64_t milliseconds) {
        int res = milliseconds/60000;
        return res;
}
    
int counter::millisToSeconds( uint64_t milliseconds) {
        int res = milliseconds/1000;
        return res % 60;
}

void counter::draw( ofTrueTypeFont &font, float posX , float posY ) {

        float _posX = posX;
        float _posY = posY;
        

        if ( started && !ended ) {
            currTime = ofGetElapsedTimeMillis();
            //ofLogNotice(__func__) << "currTime: " << currTime;
        }
            

        if ( (currTime - lastTime) > 1000 /* && (DEFAULT_VOTE_TIME >= lastTime)*/) {
            deltaTime += 1000*i;
            lastTime = currTime;
            ofLogNotice(__func__) << "startTime: " << startTime;            
        }

        int seconds = millisToSeconds(deltaTime);
        spaceSec = spaceMin = "";
        if ( seconds < 10 ) spaceSec = "0";
        int minutes = millisToMinutes(deltaTime);
        if ( minutes < 10 ) spaceMin = "0";

        if ( (deltaTime == endTime && !counterType ) || (ended /*&& counterType*/)) {
        //if ( deltaTime == 0 && !counterType ) {
            
//            endTime = deltaTime;
            ended = true;
            if (semafore) ofSetColor(ofColor::red);
                if ( i == 30) {

          //      else ofSetColor(ofColor::black);
                    semafore = !semafore;
                    ofLogVerbose(__func__) << "semafore: " << semafore;

                    i = 0;
                }
            i++;
        }

        font.drawString( spaceMin + ofToString( minutes ) + ":"  + spaceSec + ofToString( seconds ) , _posX, _posY );    
    }