#pragma once

#include "ofMain.h"
#include "ARS_reciver.h"
#include "counter.h"

class resultWindow : public ofBaseApp {

public:

     resultWindow();
virtual ~resultWindow();



    void setup();
    void update();
    void draw(float x =0 , float y = 0);
    void setCounterType ( bool stopWatch_button );
    void setStartTime ( int newStarttime );

    int numOfYes, numOfNo, numOfElse;
    int numOfVoted, numOfNotVoted, numOfAway = MAX_NUM_VOTERS;

    ofTrueTypeFont ttf20, ttf30, ttf30i, ttf40, counterFont;

    counter* countDown = new counter;

    std::string voteMode = "Nyílt";

private:

    float width;
    float textWidth;
    float height;
    ofRectangle mainRect, stringBoundingBox;

    enum VOTE_MODE{
        OPEN,
        SECRET
    };

    std::string title1;// = "Nyilt" + "szavazás:", 
    std::string title2 = "Statisztika:";
};
