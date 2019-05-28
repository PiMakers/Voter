#pragma once


#include "ofMain.h"
#include "ofxHID.h"

//ARS Reciver VID/PID   /serial
#define VENDOR_ID 1008
#define PRODUCT_ID 34836

#define MAX_NUM_VOTERS 30

class ARS_reciver : public ofThread {

public:

     ARS_reciver();
    ~ARS_reciver();

    void draw();
    void drawVote();

    bool connected;
    int res;

    std::string data;

    typedef struct _KEYPAD
    {
        // keypadModelNo??? byte 0-1
        std::string model,  data_mode, message;
        // data_mode byte 2 # 0x67 normal (vote); 0x69 exam (01-99); 0x6e chanel mode???; 0x6c join mode ???
        unsigned int digits, user_ID, keypad_No;
        long long int sent_value,  _sent_value;
        bool set;

            float width;
            float height;
            float posX, _posX;
            float posY, _posY;
            ofColor color;
        
        void clear () {
            digits = 0,
            user_ID = 0,
            keypad_No = 0;
            sent_value = 0;
            model = "";
            data_mode = "";
            message.clear();
            set = false;
        }
        
        void printInfo() {
            ofLogNotice("kepad Info: ");
            cout << "model: " << model << endl;
            cout << "data_mode: " << data_mode << endl;
            cout << "digits: " << digits << endl;
            cout << "keypad_No: " << keypad_No << endl;
            cout << "sent_value: " << sent_value << endl;
            cout << "user_ID: " << user_ID << endl;
            cout << "recived data: " << message << endl;
            cout << "set: " << set << endl;

        }
    } KEYPAD;

typedef struct _VOTE {
        
    std::string voter_name = "";
    int voterID;
    int vote_value = -1;
    bool hasVoted = false;

    int index = -1;

    float width;
    float height;
    float posX, _posX;
    float posY, _posY;
    ofColor color;
    ofTrueTypeFont vote_font;
    ofRectangle boundingBox;
                    
    void setup() {
        vote_font.load("verdana.ttf", 35, true, true);
        vote_font.setLineHeight(18.0f);
        vote_font.setLetterSpacing(1.037);
        boundingBox = vote_font.getStringBoundingBox(voter_name, 0,0);
    }

        void clear() {
            voter_name = "";
            voterID = -1;// =30;
            vote_value = -1;
            hasVoted = false;
        }

        void draw() {

            voterID = index + 1;
            width = ofGetWindowWidth()/3;
            height = ofGetWindowHeight()/15;

            posX = (voterID -1)/10;
            posY = (voterID - 10*posX) -1;

            _posX = posX * width;
            _posY = (5 + posY)*height;

 
            if ( vote_value == 1 )
                color = ofColor::green;
            else if ( vote_value == 2 )
                    color = ofColor::red;
            else if ( vote_value == 3 )
                    color = ofColor::white;
            else color = ofGetBackgroundColor();
                
                // Set color by vote
                ofFill();
                ofSetColor (color);
                ofDrawRectangle (_posX, _posY, width, height );
                // black border
                ofNoFill();
                ofSetColor(ofColor::black);
                ofDrawRectangle (_posX, _posY, width, height );
                // name
                vote_font.drawString( voter_name, _posX + 0.5*(width-boundingBox.width), _posY + height-0.5*(height- boundingBox.height));
        
        }

} VOTE /*, *PVOTE*/;

    vector <VOTE> votes;

    VOTE vote; // = new VOTE;

private:

    typedef std::vector<unsigned char> BUFFER;
    BUFFER buffer;
    
    KEYPAD keypad;
    ofxIO::HIDDevice ARS_device;
    ofxIO::HIDDeviceInfo ARS_dev_info = ofxIO::HIDDeviceInfo( VENDOR_ID, PRODUCT_ID);
    void init();
    void threadedFunction();
    void pharse_data( BUFFER data );
    void pharse_data( string &data );
    bool isReady;
};