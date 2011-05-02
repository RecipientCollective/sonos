/*
 *  
 *  sonosVideoApp
 *
 *  Created by tucano on 4/28/11.
 *  Copyright 2011. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "sonosBlob.h"
#include "functions.h"


// Uncomment to activate VideoLive
//#define _USE_LIVE_VIDEO

#define OUTPUT_HEIGHT 768.0
#define OUTPUT_WIDTH 1024.0


class sonosApp : public ofBaseApp{
	
// APP methods
public:
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

// LIBS objects
private:
	// VideoGrabber/Player
#ifdef _USE_LIVE_VIDEO
	ofVideoGrabber 		vidGrabber;
#else
	ofVideoPlayer 		vidPlayer;
#endif
	
	// openCV stuffs
	ofxCvColorImage			colorImg;
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	ofxCvContourFinder		contourFinder;
	

// INTERAL STUFFS
private:
	// Our methods
	void drawPointCloud();
	void background(int color);
	void debugDraw();
	void drawDebugInterface(float x, float y);
	void drawInterface(float x, float y);
	void setFullScreen();
	void sonosDraw();
	void inputSetup();
	void setDefaults();
	void sonosUpdate();
	void setupInterface();
	
private:
	// Our vars
	std::string filename;
	int 		camWidth;
	int 		camHeight;
	int			BlobColor;

private:
	// containers 
	map<int ,sonosBlob>  sonosblobs;
	
};
