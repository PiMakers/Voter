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

 void ARS_reciver::init(){
    auto devices = ofxIO::HIDDeviceUtils::listDevicesWithVendorAndProductIds(VENDOR_ID, PRODUCT_ID);

    for (const auto& device: devices)
    {
        connected = false;
    
        if (ARS_device.setup(device))
        {
            std::cout << device.toJSON().dump(4) << std::endl;
            std::cout << "Connected!" << std::endl;
            connected = true;
            // Only The first dev catch !!!
            //TODO: handle multiply ARS devices
            break;
        }
    }

        // this is our buffer to stroe the text data
    ofBuffer names = ofBufferFromFile("nevek.txt");
    
    if(names.size()) { 

		for (ofBuffer::Line it = names.getLines().begin(), end = names.getLines().end(); it != end; ++it) {

			string line = *it;
            
            // copy the line to draw later
            // make sure its not a empty line
            if(line.empty() == false) {
                vote.voter_name = line;
                vote.index++; 
                votes.push_back(vote);
            }
            vote.clear();
            vote.index = -1;
            // print out the line
            cout << line << endl;
            
        }
        
    }

 }

void ARS_reciver::pharse_data( BUFFER buffer ){
                
                //  buffer = _buffer;
                keypad.clear();
                char str[res];
                for (auto i = 0; i < res; i++) {
                    if (((int)buffer[i] == 6*16+6)) break;

                    sprintf(str,"%02hx ",buffer[i]);
                    keypad.message += str;                    
                    if ( i == 0 ){
                        continue;
                    }
                    else if ( i == 1 ){
                        keypad.model += keypad.message;
                            if (keypad.model.compare("aa 00 ") == 0)
                                keypad.model = "LSK CV RK";
                    }
                    /// keypad mode: vote, exam, chanel, join
                    else if ( i == 2 ) {
                        switch ( buffer[i] )
                        {
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
                            keypad.data_mode = "chanel";
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

                    else  {
                        if ( buffer[i] != 0 ) /*&& (keypad.data_mode != "command"))*/  { ///Not 00 ;
                            keypad.user_ID = 10*keypad.user_ID + buffer[i]-3*16;
                        }
                    }
                }
            keypad.set = true;

        //    keypad.printInfo();
		    
}

void ARS_reciver::drawVote () {
    if ( /*votes.size() && */isReady )
//    if (votes.size() == MAX_NUM_VOTERS)
        for (size_t i = 0; i < votes.size(); i++) {
            VOTE v = votes[i];
            v.setup();
            v.draw();
        }
}

void ARS_reciver::draw() {

//    keypad.draw();
    drawVote();
}

void ARS_reciver::threadedFunction() {

    while(isThreadRunning()){

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
                    if ( keypad.data_mode.compare("vote") == 0) {
                        if (vote.index < MAX_NUM_VOTERS-1) {
                            if ( ! vote.hasVoted ) {
                                vote.index ++;
                                if (! votes[vote.index].voter_name.empty()) vote.voter_name = votes[vote.index].voter_name;
                                else vote.voter_name = " Not set !! " + ofToString(keypad.user_ID);
                                
                                vote.vote_value = keypad.sent_value;
                                vote.voterID = keypad.user_ID;
                                vote.hasVoted = true;
                                
                                votes.push_back(vote);
                                vote.clear();
                                isReady = false;
                            }
                        }
                        else isReady = true;

                    }
                //    else cout << "NOT A VOTE!\n" <<  vote.index << endl;
                }

            /*
                keypad.clear();
                char str[res];
                for (auto i = 0; i < res; i++) {
                    if (((int)buffer[i] == 6*16+6)) break;

                    sprintf(str,"%02hx ",buffer[i]);
                    keypad.message += str;                    
                    if ( i == 0 ){
                        continue;
                    }
                    else if ( i == 1 ){
                        keypad.model += keypad.message;
                            if (keypad.model.compare("aa 00 ") == 0)
                                keypad.model = "LSK CV RK";
                    }
                    /// keypad mode: vote, exam, chanel, join
                    else if ( i == 2 ) {
                        switch ( buffer[i] )
                        {
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
                            keypad.data_mode = "chanel";
                            break;
                        default:
                            cout << "DATAMODE =" << (int)buffer[i] << endl;
                            keypad.data_mode = "unknown";
                            break;
                        }
                    }
                    /// size of databuffer (num of digits)
                    else if ( i == 3 ) {
                        keypad.digits = buffer[i];
                        if ( keypad.digits >= 15 )
                            keypad.digits = keypad.digits -15;
                        else if ( keypad.digits == 4 ) {
                        //      keypad.isMaster = true;
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
                    else  {
                        if ( buffer[i] != 0 ) { ///Not 00 ;
                            keypad.user_ID = 10*keypad.user_ID + buffer[i]-3*16;
                        }
                    }
                }

            keypad.printInfo();
            */
		    
        }
        else 
        {
            std::cout << "OPENING...." << std::endl;
            init();
        }
        sleep (200);
    }

 }