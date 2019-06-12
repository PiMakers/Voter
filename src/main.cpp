//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


int main()
{
/*
    ofSetupOpenGL(640, 480, OF_WINDOW);
    return ofRunApp(std::make_shared<ofApp>());
*/

	ofGLFWWindowSettings settings;
	settings.windowMode = OF_FULLSCREEN;
	settings.monitor = 0;
	auto window1 = ofCreateWindow(settings);
	auto app1 = std::make_shared<ofApp>();
/*

	settings.monitor = 1;
	auto window2 = ofCreateWindow(settings);
	auto app2 = std::make_shared<OtherApp>();
*/
	ofRunApp(window1, app1);
//	ofRunApp(window2, app2);
	ofRunMainLoop();

}