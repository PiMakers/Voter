#pragma once

#include "ofMain.h"
#include "settings.h"


class counter /*: ofBaseApp */{

public:

typedef enum _COUNTER_TYPE{
    COUNT_DOWN,
    STOP_WATCH
} COUNTER_TYPE;

     counter(){
         startTime = DEFAULT_VOTE_TIME;
         counterType = COUNT_DOWN ;
         stopped = true;
     };

    ~counter(){};

    void start();

    void stop();

    bool isStarted();
    
    void setCounterType(int ct);
    
    void setStartTime ( int newStartTime );

    void draw( ofTrueTypeFont &font, float posX = 0.0, float posY = 0.0 );

    int currTime,lastTime, startTime, endTime;

    bool ended;

    bool stopped;

    COUNTER_TYPE counterType;    

private:

        int millisToMinutes( uint64_t milliseconds);
        int millisToSeconds( uint64_t milliseconds);
        
        int deltaTime = DEFAULT_VOTE_TIME;
        bool semafore = true;
        string spaceSec, spaceMin;

        int i;
        bool started = false;
};