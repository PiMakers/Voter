#pragma once

#include "ofMain.h"
#include "ARS_reciver.h"

class resultWindow : public ofBaseApp {

public:

     resultWindow();
    ~resultWindow();



    void setup();
    void update();
    void draw(float x =0 , float y = 0);
private:

    float width;
    float textWidth;
    float height;
    ofRectangle mainRect;

    std::string title = "Nyilt szavazás:";
    ofTrueTypeFont ttf20, ttf30, ttf40;
    ARS_reciver* arsReciver;// = new ARS_reciver;
    /// GUI::bBox = font.getStringBoundingBox(title, 0,0);
};
