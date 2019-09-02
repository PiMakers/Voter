#include "ofxARS_reciver.h"


 ofxARS_reciver::ofxARS_reciver(){

    if (hid_init()) {
        ofLogError("HIIIDinit FAILED!!!");
        return;
    }
    ofLogNotice("ARS STARTED!!!!!!");
}
 
 ofxARS_reciver::~ofxARS_reciver() {
    ofLogNotice("ARS EXITED!!!!!!");
    hid_close(ARS_device);
    hid_exit();
}

void ofxARS_reciver::init(){

    connected = false;
    hid_close(ARS_device);
    ARS_device = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
    if (!ARS_device ){
        ofLogError(__func__) << "Unable to open ARS device!";
        return;
    }
    // Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(ARS_device, 1);
    connected = true;
    loadNames();
}

void ofxARS_reciver::loadNames () {
    if (!names_loaded) {
        // this is our buffer to strore the text data
        ofFile name("nevek.txt");
        if ( !name.exists() ){
            ofLogError(__func__) << "data/nevek.txt nem található vagy sérült!";
            return;
        }
        ofBuffer names = ofBufferFromFile("nevek.txt");
        ofLogVerbose() << "names.size(): " << names.size() << "\t\tnames_loaded: " << names_loaded << endl;    
        if( names.size() ) {
            vector <string> data;

            for (ofBuffer::Line it = names.getLines().begin(), end = names.getLines().end(); it != end; ++it) {

                string line = *it;
                
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
        }
        else {
            ofLogError(__func__) << "data/nevek.txt nem található vagy sérült!";
            return;
        }
    }
}

void ofxARS_reciver::pharse_data( BUFFER* buffer ){
    keypad.clear();

    //char str[res];
    char str[4];
    for (int i = 0; i < res; i++) {
        
        if ( buffer[i] == 0x66 ) { // ff Data end
            break;
        }

        sprintf(str,"%02hx ",buffer[i]);

            if ( i == 0 ){
                keypad.model += str;
            }
            
            else if ( i <= 1 ){
                
                keypad.model += str;//keypad.data;
                
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

                    case 0x67:  //103
                        keypad.data_mode = "vote";
                        break;
                    case 0x69:  //105
                        keypad.data_mode = "exam";
                        break;
                    case 0x6c:  //108
                        keypad.data_mode = "join";
                        break;
                    case 0x6e:  //110
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
            else break;

            keypad.data += str;
            
    }
    
    ofSendMessage(keypad.data);
}

void ofxARS_reciver::handel_data() {
    if ( keypad.data_mode.compare("vote") == 0 && vote_started && !vote_ended ) {

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
                    ofSendMessage("UP_ARROW");
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

            case  FORM_BTN :
                    
                    ofLogNotice(__func__) << "FORM_BTN: " << keypad.sent_value;
                    break;

            case  STOP_WATCH_BTN :
                    if (!vote_started)
                        stopWatch_button = !stopWatch_button;
                    ofLogNotice(__func__) << "STOP_WATCH_BTN: " << keypad.sent_value;
                    ofSendMessage("STOP_WATCH_BTN");
                    break;

            case  OK_BTN :
                    
                    ofLogNotice(__func__) << "OK_BTN: " << keypad.sent_value;
                    break;

            case  DISPLAY_BTN :
                    
                    ofLogNotice(__func__) << "DISPLAY_BTN: " << keypad.sent_value;
                    break;

            case  VOTE_BTN :
                
                ofLogNotice(__func__) << "VOTE_BTN: " << keypad.sent_value;
                break;
            
            case  PAUSE_BTN :
                
                ofLogNotice(__func__) << "PAUSE_BTN:  " << keypad.sent_value;
                break;
            
            case  ESC_BTN :
                
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

void  ofxARS_reciver::resetVoteResult() {
        for (size_t i = 0; i < votes.size(); i++) {
            votes[i].clearVoteResult();
        }
}

void ofxARS_reciver::update() {
    if (ARS_device) {
        res = hid_read(ARS_device, buffer, sizeof(buffer) );
                    
        if (res == 0)
            printf("waiting...\r");
                
        else if (res < 0) {
            printf("Unable to read!\r");
            init();
        }
        else if ( res >= 1 ) {
            pharse_data ( buffer );
            handel_data();
        }
    }
    else {
        std::cout << "Opening...                  \r";
        init();
    }
}