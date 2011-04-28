#include "ofMain.h"
#include "sonosApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
	
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, OUTPUT_WIDTH, OUTPUT_HEIGHT, OF_WINDOW);			// <-------- setup the GL context
	
	//antialias settings
	window.setGlutDisplayString("rgba double samples >= 4 depth");
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new sonosApp());
	
}