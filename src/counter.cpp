#include "counter.h"


void counter::setCounterType ( int ct ) {
    if (stopped) {
        if ( ct == COUNT_DOWN ) {
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
        if (newStartTime < 0)
                newStartTime = 0;
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
            i=1;
        }    
        started = false;
        ended = true;
        stopped = true;
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
        }
            

        if ( (currTime - lastTime) > 1000 ) {
            deltaTime += 1000*i;
            lastTime = currTime;
            ofLogVerbose(__func__) << "startTime: " << startTime;            
        }

        int seconds = millisToSeconds(deltaTime);
        spaceSec = spaceMin = "";
        if ( seconds < 10 ) spaceSec = "0";
        int minutes = millisToMinutes(deltaTime);
        if ( minutes < 10 ) spaceMin = " ";

        if ( (deltaTime == endTime && !counterType ) || ended) {
            
            if (!ended) {
                ended = true;
                ofSendMessage("COUNTDOWN_ENDED");
            }
            if (semafore) ofSetColor(ofColor::red);
                if ( i == 30) {
                    semafore = !semafore;
                    i = 0;
                }
            i++;
        }

        font.drawString( spaceMin + ofToString( minutes ) + ":"  + spaceSec + ofToString( seconds ) , _posX, _posY );    
    }