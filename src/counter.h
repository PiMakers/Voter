#pragma once

#include "ofMain.h"

#define DEFAULT_VOTE_TIME 30000

class counter : ofBaseApp {

public:

typedef enum _COUNTER_TYPE{
    COUNT_DOWN,
    STOP_WATCH
} COUNTER_TYPE;

     counter( COUNTER_TYPE ct = COUNT_DOWN ){
         startTime = DEFAULT_VOTE_TIME;

     };
    ~counter(){};
    
    void setup(){};
    
    void update() {};

    void start();

    void stop();

    bool isStarted();
    
    void setCounterType(int ct);
    
    void setStartTime ( int newStartTime );

    int millisToMinutes( uint64_t milliseconds);
    
    int millisToSeconds( uint64_t milliseconds);

    void draw( ofTrueTypeFont &font, float posX = 0.0, float posY = 0.0 );

    int currTime,lastTime, startTime, endTime;

    bool ended = false;

    bool stopped = true;

        COUNTER_TYPE counterType;    

private:

        
//        int counterType = COUNT_DOWN;
        int deltaTime = DEFAULT_VOTE_TIME;
        bool semafore = true;
        string spaceSec, spaceMin;



        int i;// = -1;
        bool started = false;
};