#pragma once

#include "ofMain.h"
#include "settings.h"
#include "counter.h"

class resultWindow : public ofBaseApp {

public:

     resultWindow();
    ~resultWindow();



    void setup(float scale = 1.0);
    void update();
    void draw(float x =0 , float y = 0, float scaleW = 1.0, float scaleH = 1.0);
    void setCounterType ( bool stopWatch_button );
    void setStartTime ( int newStarttime );
    void reset();

    int numOfYes, numOfNo, numOfElse;
    int numOfVoted, numOfNotVoted, numOfAway = MAX_NUM_VOTERS;

    ofTrueTypeFont ttf20, ttf30, ttf30i, ttf40, counterFont;

    counter* countDown = new counter;
    //counter countDown;

    std::string voteMode = "Nyílt";

private:

    float width;
    float textWidth;
    float height;
    ofRectangle mainRect, stringBoundingBox;

    string translateDayName();

    enum VOTE_MODE{
        OPEN,
        SECRET
    };

    std::string title;
};
