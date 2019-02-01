//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"

#define VENDOR_ID 1008
#define PRODUCT_ID 34836

ofxIO::HIDDevice _device;
int res=0;
std::vector<uint8_t> buffer;
bool semafore = true;
string message;

#define MAX_STR 255
wchar_t wstr[MAX_STR];
///hid_device *handle;
int i;

void ofApp::setup()
{
///    ofSetLogLevel(OF_LOG_VERBOSE);
    
//  auto devices = ofxIO::HIDDeviceUtils::listDevices();
    auto devices = ofxIO::HIDDeviceUtils::listDevicesWithVendorAndProductIds(VENDOR_ID, PRODUCT_ID);

    for (const auto& device: devices)
    {
        std::cout << device.toJSON().dump(4) << std::endl;
        
///        ofxIO::HIDDevice _device;

        if (_device.setup(device))
        {
            std::cout << "Success." << std::endl;
        }
        else
        {
            std::cout << "Failed." << std::endl;
        }

    }

}

void ofApp::update() {
/*
        if (_device.isOpen()) {std::cout << "IsOpen!!!!" << std::endl;}
        else std::cout << "Failed." << std::endl;
        usleep(500);
        std::streamsize read(std::vector<uint8_t>& buffer,
                         std::size_t readBufferSize = DEFAULT_READ_BUFFER_SIZE);
        */
 //       while (res == 0) {
//        _device.read
		res = _device.read( buffer );
		if (res == 0)
			printf("waiting...\r");
		else if (res < 0) {
			printf("Unable to read()\r");
            setup();
        }
/*
		else {
		// Print out the returned buffer.
		printf ("RECIVED: %i\n", res);
		for (i = 0; i < 10; i++)
		printf("%02hhx ", buffer[i]);
		printf("\n");
		}
//		res=0;
		#ifdef WIN32
		Sleep(500);
		#else
		usleep(500*1000);
		#endif
        */
//	}
}

void ofApp::exit()
{
}


void ofApp::draw()
{

    	if ( res < 0  ) {
            if (semafore) {
            ofSetColor(255,0,0);
            ofDrawCircle(ofGetWindowWidth()/2, ofGetWindowHeight()/2, ofGetWindowWidth()/3);
            }
            #ifdef WIN32
		    Sleep(500);
		    #else
		    usleep(500*1000);
		    #endif
            semafore=!semafore;
        }
        else {
            if ( res >= 1 ) {
    		// Print out the returned buffer.
    		printf ("RECIVED: %i\n", res);
        		for (i = 0; i < res; i++) {
        		char str[res];
                sprintf(str,"%02hhx ",buffer[i]);
                 message+= str;
                }
            message+="\n";
		    }
        
            ofSetColor(0);
            ofDrawBitmapString(message,100,100);
            ofLogVerbose(__func__, message);
        }
//        message.clear();
}
