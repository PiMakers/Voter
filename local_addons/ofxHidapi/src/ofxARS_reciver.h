#pragma once


#include "ofMain.h"
#include "settings.h"

//ARS Reciver VID/PID   /serial
#define VENDOR_ID 1008
#define PRODUCT_ID 34836

// KeyPad Buttons:
#define     UP_ARROW         1
#define     DOWN_ARROW       2
#define     LEFT_ARROW       3
#define     RIGHT_ARROW      4
#define     STOP_BTN         5
#define     PLAY_BTN         6
#define     PPT_BTN          7
#define     GRAF_BTN         8
#define     FORM_BTN         9
#define     STOP_WATCH_BTN  10
#define     OK_BTN          11
#define     DISPLAY_BTN     15
#define     VOTE_BTN        16
#define     PAUSE_BTN       17
#define     ESC_BTN         18

#ifndef MAX_NUM_VOTERS
    #define MAX_NUM_VOTERS 30
#endif

#include <hidapi.h>    

class ofxARS_reciver {

public:

     ofxARS_reciver();
    ~ofxARS_reciver();

    void update();
    void resetVoteResult();
    void drawCounter(ofTrueTypeFont counterFont, float X = 0, float Y = 0 );
    void startCounter();
    void stopCounter();

    bool connected, vote_started = false, graf_button = false, stopWatch_button = false, vote_ended = true;
    int udArrow = 0, lrArrow = 0;
    int res;

    typedef struct _KEYPAD {
        // keypadModelNo??? byte 0-1
        std::string model,  data_mode, data;
        // data_mode byte 2 # 0x67 normal (vote); 0x69 exam (01-99); 0x6e chanel mode???; 0x6c join mode ???
        unsigned int keypad_No;
        int digits, user_ID;
        long long int sent_value;
        
        void clear () {
            digits = 0,
            user_ID = 0,
            keypad_No = 0;
            sent_value = 0;
            model = "";
            data_mode = "";
            data.clear();
        }
        
        void printInfo() {
            ofLogNotice("kepad Info: ");
            cout << "model: " << model << endl;
            cout << "data_mode: " << data_mode << endl;
            cout << "digits: " << digits << endl;
            cout << "keypad_No: " << keypad_No << endl;
            cout << "sent_value: " << sent_value << endl;
            cout << "user_ID: " << user_ID << endl;
            cout << "recived data: " << data << endl;
        }
    } KEYPAD;

    typedef struct _VOTE {
            
        std::string voter_name = "";
        int voterID = -1, index = -1;
        int vote_value = -1;
        bool hasVoted = false, registered = false;

        void clear() {
                voter_name = "";
                voterID = -1;
                vote_value = -1;
                hasVoted = false;
                registered = false;
            }
        void clearVoteResult() {
                vote_value = -1;
                hasVoted = false;
            }

        } VOTE /*, *PVOTE*/;


    vector <VOTE> votes;

    VOTE vote;

private:

    KEYPAD keypad;
    
    hid_device *ARS_device;

    typedef unsigned char BUFFER;
    BUFFER buffer[63];
    void pharse_data( BUFFER* data );
    void handel_data();

    void init();
    void loadNames ();
    
    bool names_loaded;
    std::string data;
};