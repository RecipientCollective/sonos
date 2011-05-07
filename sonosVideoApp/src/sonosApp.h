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
#include "MSAPhysics2D.h"

// Uncomment to activate VideoLive
//#define _USE_LIVE_VIDEO

#define OUTPUT_HEIGHT 768.0
#define OUTPUT_WIDTH 1024.0

// PHYSICS
#define	GRAVITY	-0.1
#define SECTOR_COUNT 1		// currently there is a bug at sector borders
#define NODE_MIN_RADIUS			1
#define NODE_MAX_RADIUS			5
#define MIN_MASS				1
#define MAX_MASS				3
#define MIN_BOUNCE				0.2
#define MAX_BOUNCE				0.9

// BITMASK sonosUpdate
#define SONOSEMPTY        1  // 2^0, bit 0
#define BLOBSEMPTY        2  // 2^1, bit 1
#define MORESONOS         4  // 2^2, bit 2
#define MOREBLOBS         8  // 2^3, bit 3
#define EQUALSIZE        16  // 2^4, bit 4

using namespace MSA;

class sonosApp : public ofBaseApp{

// STATUS UPDATE ENUM
public:
	unsigned char flags;    // 8-bit MAX flag
	unsigned char pflags;   // 8-bit flags for previuous state
	
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
	
	Physics::World2D		physics;
	
	// MSAFluids
	//MSA::FluidSolver	fluidSolver;
//	MSA::FluidDrawerGl	fluidDrawer;	
//	ParticleSystem		particleSystem;
	
	//images containers
	ofImage lettering1;

// INTERAL STUFFS
private:
	// Our methods
	void inputSetup();                              // setup INPUTS
	void setDefaults();                             // setup VARS
	void setupInterface();                          // setup INTERFACE
	void setupPhysicsWorld();                       // setup PYSICS
	void sonosDraw();
	void sonosUpdate();
	void sonosBlobsInsert();
	void sonosBlobsUpdate();	
	void background(int color);
	void debugDraw();
	void drawDebugInterface(float x, float y);
	void drawInterface(float x, float y);
	void setFullScreen();
	
private:
	// Our vars
	std::string filename;
	int 		camWidth;
	int 		camHeight;
	int			BlobColor;
		
private:
	// containers 
	map<string ,sonosBlob>  sonosblobs;
	
};
