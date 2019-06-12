#pragma once


#include "ARS_reciver.h"


 ARS_reciver::ARS_reciver(){
    startThread();
    ofLogNotice("ARS STARTED!!!!!!");
    ofLogNotice();
}
 
 ARS_reciver::~ARS_reciver(){

    ///     stopThread();
     waitForThread();
     if (ARS_device.isOpen()) {

         ARS_device.close();
     }
 };
/*
void ARS_reciver::setup() {

	ttf20.load("verdana.ttf", 10, true, true);
    ttf20.setLineHeight(18.0f);
	ttf20.setLetterSpacing(1.037);


    //        ofTrueTypeFont::setGlobalDpi(72);
    while (!ttf20.isLoaded()) {

    ofTrueTypeFontSettings settings("verdana.ttf", 10);
    settings.addRange(ofUnicode::LatinA);
    ttf20.load(settings);
    ttf20.drawString( "BBBBBBBBBBBBBRRRRRRR" , 300, 500);
    sleep (500);

    }
    
        vote_font.load("verdana.ttf", 10); //, true, true);
        vote_font.setLineHeight(18.0f);
        vote_font.setLetterSpacing(1.037);

    ofLogError(__func__);
    sleep (500);
    }
    
}

void ARS_reciver::draw() {
    //    if (votes.size() == MAX_NUM_VOTERS)
        for (size_t i = 0; i < votes.size(); i++) {
            VOTE v = votes[i];
        drawVote (ttf20,  v);
        cout << "NAME: " << votes[i].voter_name << endl;
        }

}
*/

void ARS_reciver::init(){

    connected = false;
    auto devices = ofxIO::HIDDeviceUtils::listDevicesWithVendorAndProductIds(VENDOR_ID, PRODUCT_ID);

    for (const auto& device: devices) {
        if (ARS_device.setup(device))
        {
            // ofxHID bug: Windows Throw an Error at toMultiByte()
            #ifndef WIN32
                std::cout << device.toJSON().dump(4) << std::endl;
            #endif
            std::cout << "Connected!" << std::endl;
            connected = true;
            // Only The first dev catch !!!
            //TODO: handle multiply ARS devices
            break;
        }
    }
    
        // this is our buffer to strore the text data
    ofBuffer names = ofBufferFromFile("nevek.txt");
    cout << "names.size(): " << names.size() << "\t\tnames_loaded: " << names_loaded << endl;    
    if( names.size() && !names_loaded ) {
        vector <string> data;

		for (ofBuffer::Line it = names.getLines().begin(), end = names.getLines().end(); it != end; ++it) {

			string line = *it;

            
            // copy the line to draw later
            // make sure its not an empty line
            if((line.empty() == false) ) {
                data = ofSplitString(line, ";");
                vote.voter_name = data[0];
                vote.voterID = std::stoi(data[1]);
                vote.index++; 
                votes.push_back(vote);
                data.clear();
                cout << "voter_name: " << vote.voter_name << "\t\tvoterID: " << vote.voterID << endl;
                cout << "index: " << vote.index << endl;
            }
            vote.clear();
        }
        names_loaded = true;
        vote.index = -1;
    //        sleep(5000);
    //        if ( !ttf20.isLoaded() ) setup();
    }

}

void ARS_reciver::pharse_data( BUFFER buffer ){
                
    keypad.clear();
    char str[res];
     
    for (auto i = 0; i < res; i++) {
        
        if ( buffer[i] == 0x66 ) { // Data end
            break;
        }

        sprintf(str,"%02hx ",buffer[i]);
        keypad.data += str;                    
                
            if ( i == 0 ){
                continue;
            }
            
            else if ( i == 1 ){
                
                keypad.model += keypad.data;
                
                if (keypad.model.compare("aa 00 ") == 0)
                        keypad.model = "LSK CV RK";
                else {
                    ofLogError(__func__) << "Unknown Keypad type";
                    break;
                }
            }
                /// keypad mode: vote, exam, chanel, join
            else if ( i == 2 ) {
                
                switch ( buffer[i] ) {

                        case 103: // 67
                            keypad.data_mode = "vote";
                            break;
                        case 105: // 69
                            keypad.data_mode = "exam";
                            break;
                        case 108: // 6c
                            keypad.data_mode = "join";
                            break;
                        case 110: // 6e
                            keypad.data_mode = "channel";
                            break;
                        default:
                            cout << "DATAMODE =" << (int)buffer[i] << endl;
                            keypad.data_mode = "unknown";
                            break;
                }
            }
                /// size of databuffer (num of digits)
            else if ( i == 3 ) {
                
                keypad.digits = /*(int)*/buffer[i];
                
                if ( keypad.digits >= 15 )
                    keypad.digits = keypad.digits -15;
                /*
                else if ( keypad.digits == 12 ) { // join_mode
                    keypad.digits = 0;
                }
                */
                else if ( keypad.digits == 4 ) {
                    keypad.data_mode = "command";
                    keypad.digits = 1;
                }                
                    //      uint8_t a0 = a & 0xff;  // a0 = 0x02
                    //      uint8_t a1 = a >> 8;    // a1 = 0x60   else break;
            }
            /// keypadNumber.
            else if ( i <= 6 ) {
                        keypad.keypad_No = 256*keypad.keypad_No + buffer[i];
            }
            
            else if ( i < keypad.digits + 7 ) {
                
                keypad.sent_value = 10*keypad.sent_value + buffer[i];
                if ( keypad.sent_value > 48 ) {  // master keyboard send int others send char!!!
                        keypad.sent_value = keypad.sent_value - 3*16;
                }
            }

            else  if ( buffer[i] != 0 ) /*&& (keypad.data_mode != "command"))*/  { ///Not 00 ;
                            keypad.user_ID = 10*keypad.user_ID + buffer[i]-3*16;
            }
            
    }
}

void ARS_reciver::handel_data() {
    if ( keypad.data_mode.compare("vote") == 0 && vote_started ) {

        for (size_t i = 0; i < votes.size(); i++) {

            if (vote.index < MAX_NUM_VOTERS-1) {
                bool enable_votecorrection = false;
                
                if ( ! votes[i].hasVoted || enable_votecorrection ) {
                    
                    if ( votes[i].voterID == keypad.user_ID ) {
                        votes[i].vote_value = keypad.sent_value;
                        votes[i].hasVoted = true;

                        ofLogVerbose(__func__) << " voter_name = " << votes[i].voter_name;
                        ofLogVerbose(__func__) << " voterID = " << votes[i].voterID;
                        ofLogVerbose(__func__) << " keypad.user_ID = " << keypad.user_ID;
                        ofLogVerbose(__func__) << " i = " << i;

                        vote.clear();
                        break;
                    }
                }
            }
        }
    }
                    
    else if ( keypad.data_mode.compare("command") == 0) {

        switch (keypad.sent_value) {

            case  UP_ARROW :
                    if (!vote_started)
                        udArrow++;
                    ofLogNotice(__func__) << "UP_ARROW: " << keypad.sent_value;
                    break;
            
            case  DOWN_ARROW :
                    if (!vote_started)
                        if (10000*udArrow + 60000 * lrArrow > -DEFAULT_VOTE_TIME ) udArrow--;
                    
                    ofLogNotice(__func__) << "DOWN_ARROW: " << keypad.sent_value;
                    break;
            
            case  LEFT_ARROW :
                    if (!vote_started)
                        if (60000 * lrArrow > -DEFAULT_VOTE_TIME ) lrArrow--;
                    ofLogNotice(__func__) << "LEFT_ARROW: " << keypad.sent_value;
                    break;

            case  RIGHT_ARROW :
                    if (!vote_started)
                        lrArrow++;
                    ofLogNotice(__func__) << "RIGHT_ARROW: " << keypad.sent_value;
                    break;

            case  STOP_BTN :
                    
                    ofLogNotice(__func__) << "STOP_BTN: " << keypad.sent_value;
                    if (vote_ended) {
                        vote_started = false;
                        ofLogNotice(__func__) << "Vote_stopped!";
                    }
                    else {
                        vote_ended = true;
                        ofLogNotice(__func__) << "Vote_ended!";
                    }
                    break;

            case  PLAY_BTN :
                    
                    ofLogNotice(__func__) << "PLAY_BTN: " << keypad.sent_value;
                        if (vote_ended) {
                        vote_started = true;
                        vote_ended = false;
                        }
                    break;

            case  PPT_BTN :
                    
                    ofLogNotice(__func__) << "PPT_BTN:  " << keypad.sent_value;
                    break;

            case  GRAF_BTN :
                    if (!vote_started)
                        graf_button = !graf_button;
                    ofLogNotice(__func__) << "GRAF_BTN:  " << keypad.sent_value;
                    break;

            case  9 :
                    
                    ofLogNotice(__func__) << "FORM_BTN: " << keypad.sent_value;
                    break;

            case  STOP_WATCH_BTN :
                    if (!vote_started)
                        stopWatch_button = !stopWatch_button;
                    ofLogNotice(__func__) << "STOP_WATCH_BTN: " << keypad.sent_value;
                    break;

            case  11 :
                    
                    ofLogNotice(__func__) << "OK_BTN: " << keypad.sent_value;
                    break;

            case  15 :
                    
                    ofLogNotice(__func__) << "DISPLAY_BTN: " << keypad.sent_value;
                    break;

            case  16 :
                
                ofLogNotice(__func__) << "VOTE_BTN: " << keypad.sent_value;
                break;
            
            case  17 :
                
                ofLogNotice(__func__) << "PAUSE_BTN:  " << keypad.sent_value;
                break;
            
            case  18 :
                
                ofLogNotice(__func__) << "ESC_BTN" << keypad.sent_value;
                break;

            default :
            
                ofLogNotice(__func__) << "UNKNOWN_BUTTON:  " << keypad.sent_value;
                                ofLogNotice(__func__) << "KeyPAdCommand:  " << keypad.sent_value;
        }
    }

    else if ( keypad.data_mode.compare("join") == 0) {
        ofLogVerbose(__func__) << "Join command recived";
        for (size_t i = 0; i < votes.size(); i++) {

            if (vote.index < MAX_NUM_VOTERS-1) {
                
                    if ( votes[i].voterID == keypad.user_ID ) {
                        votes[i].registered =true;
                        
                        ofLogVerbose(__func__) << " voter_name = " << votes[i].voter_name;
                        ofLogVerbose(__func__) << " voterID = " << votes[i].voterID;
                        ofLogVerbose(__func__) << " keypad.user_ID = " << keypad.user_ID;
                        ofLogVerbose(__func__) << " i = " << i;

                        vote.clear();
                        break;
                    }
            }
        }
    }

    else if ( keypad.data_mode.compare("channel") == 0) {
        ofLogVerbose(__func__) << "Channel command recived";
    }

    else if ( keypad.data_mode.compare("exam") == 0) {
        ofLogVerbose(__func__) << "Exam command recived";
        ofLogVerbose(__func__) << keypad.data;
    }

    else {
        ofLogVerbose(__func__) << "Unknown command recived";
        ofLogVerbose(__func__) << keypad.data;
    }
}

void  ARS_reciver::resetVoteResult() {
        for (size_t i = 0; i < votes.size(); i++) {
            votes[i].clearVoteResult();
        }
}

void ARS_reciver::threadedFunction() {

    while(isThreadRunning()) {

        if ( ARS_device.isOpen()) {
            res = ARS_device.read( buffer );
            if (res == 0)
                printf("waiting...\r");
                
            else if (res < 0) {
                printf("Unable to read()\r");
                init();
            }
            else if ( res >= 1 ) {
                pharse_data ( buffer );
                handel_data();
            }

        }
        else 
        {
            std::cout << "OPENING...." << std::endl;
            init();
        }
        sleep (200);
    }

 }