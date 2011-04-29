/*
 *  sonosAppMethods.cpp
 *  sonosVideoApp
 *
 *  Created by tucano on 4/29/11.
 *  Copyright 2011 IFOM-FIRC. All rights reserved.
 *
 */

#include "sonosApp.h"

//--------------------------------------------------------------
//   INTERNAL METHODS of sonosApp
//
//   
//--------------------------------------------------------------

//--------------------------------------------------------------
void sonosApp::inputSetup()
{
#ifdef _USE_LIVE_VIDEO
	camWidth = 640;
	camHeight = 480;
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth, camHeight, false);
#else
	// FILENAME
	filename = "videos/retro-ir.mov";
	
	//check if file exists
	bool bFileThere = false;
	fstream fin;
	string fileNameInOF = ofToDataPath(filename, false); // since OF files are in the data directory, we need to do this
	fin.open(fileNameInOF.c_str(),ios::in);
	if ( fin.is_open() ) {
#ifdef DEBUG
		cerr << "Founded file " << fileNameInOF << endl;
#endif
		bFileThere =true;
	}
	fin.close();
	
	if (bFileThere) {
		vidPlayer.loadMovie(filename);
		vidPlayer.play();
		camWidth = vidPlayer.getWidth();
		camHeight = vidPlayer.getHeight();
	} else {
#ifdef DEBUG		
		cerr << "File " << fileNameInOF << " is not here!" << endl;
#endif
	sonosApp:exit();
	}
#endif
	
#ifdef DEBUG		
	cerr << "Input size: width =" << camWidth << " height = " << camHeight << endl;
#endif	
}

//--------------------------------------------------------------

void sonosApp::setDefaults()
{
	// ALLOCATE IMAGES SIZES
	colorImg.allocate(camWidth, camHeight);
	grayImage.allocate(camWidth, camHeight);
	grayBg.allocate(camWidth,camHeight);
	grayDiff.allocate(camWidth,camHeight);
	
	// SETUP DEFAULT PARAMETERS
	bLearnBakground = true;
	Threshold = 50;
	ofSetCircleResolution(40); 	   //set resolution of circle
	ofEnableSmoothing();	
	ofSetFrameRate(60);
	
	//for smooth animation, set vertical sync if we can
	//ofSetVerticalSync(true);
	
	//colors setup
	BckColor=1;
	BlobColor=0xDD00CC;
	
	blobMax=2;
	contour_min = 350;
	scale_x = 1.0;
	scale_y = 1.0;
	mtrx = 1;
	mtry = 1;
	interface = true;
	circle = false;
	debug = false;
	rectangle = false;
	box = true;
	avatar = false;
}

//--------------------------------------------------------------
void sonosApp::sonosUpdate()
{
	// 1. SET IMAGE from current pixels, move to greyImg, if learn background do it
#ifdef _USE_LIVE_VIDEO
	colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
#else
	colorImg.setFromPixels(vidPlayer.getPixels(), camWidth,camHeight);
#endif		
	grayImage = colorImg;
	if (bLearnBakground == true){
		grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
		bLearnBakground = false;
	}
	
	// take the abs value of the difference between background and incoming and then threshold:
	grayDiff.absDiff(grayBg, grayImage);
	grayDiff.threshold(Threshold);
	
	//flag updated the cv image
	grayImage.flagImageChanged();
	grayDiff.flagImageChanged();
	
	/* COUNTOUR FINDER FUNCTION
	 
	 ofxCvContourFinder::findContours( 
	 ofxCvGrayscaleImage&  input,
	 int minArea,
	 int maxArea,
	 int nConsidered,
	 bool bFindHoles,
	 bool bUseApproximation
	 )
	 
	 find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
	 also, find holes is set to true so we will get interior contours as well...  */
	contourFinder.findContours(grayDiff, contour_min, (camWidth*camHeight)/3, blobMax, false, true);
	
	// sort by centroid
	std::sort(contourFinder.blobs.begin(),contourFinder.blobs.end(), sortByCentroid);
	
	// clean sonosblob map
	sonosblobs.clear();
	
	// copy the blobs to sonosBlobs map
	for(int i = 0; i < contourFinder.blobs.size(); i++) {
		
		sonosBlob myblob = contourFinder.blobs[i];
		sonosblobs.insert(std::pair<int, sonosBlob>(i,myblob));
	}	
}

//--------------------------------------------------------------
void sonosApp::sonosDraw()
{	
	// background
	sonosApp::background(BckColor);
	
	ofPushMatrix();
	ofScale(scale_x, scale_y, 1.0);
	ofTranslate(mtrx, mtry, 1.0);
	
	if (box) {
		ofPushStyle();
		ofSetColor(255, 255, 255);
		ofNoFill();
		ofRect(0, 0, camWidth, camHeight);
		ofPopStyle();
	}
	
	// in draw we iterate in the map
	for(map<int, sonosBlob>::iterator i = sonosblobs.begin(); i != sonosblobs.end(); ++i)
	{
		sonosBlob curr_blob = i->second;
		curr_blob.setBlobColor(BlobColor);
		curr_blob.drawPixels();
		
		// IF interface: draw rectangle around 
		if(interface) curr_blob.drawRect();
		
		// set color by position (TEST)
		ofPushStyle();
		if (i->first == 0) {
			ofSetColor(255, 0, 0);
		} else if (i->first == 1) {
			ofSetColor(0, 255, 0);
		} else if (i->first == 2) {
			ofSetColor(0, 0, 255);
		} else {
			ofSetColor(100, 100, 150);
		}
		
		if (circle) curr_blob.circle();
		if (rectangle) curr_blob.rectangle();
		ofPopStyle();
		
		// test AVATAR
		if (avatar) curr_blob.drawAvatar(particleSystem);
	}
	
	ofPopMatrix();
}

//--------------------------------------------------------------
void sonosApp::setFullScreen(bool full)
{
	if (full) {
		ofSetFullscreen(true);
	} else {
		ofSetWindowShape(OUTPUT_WIDTH,OUTPUT_HEIGHT);
		ofSetFullscreen(false);
		// figure out how to put the window in the center:
		int screenW = ofGetScreenWidth();
		int screenH = ofGetScreenHeight();
		ofSetWindowPosition(screenW/2-OUTPUT_WIDTH/2, screenH/2-OUTPUT_HEIGHT/2);
	}
}

//--------------------------------------------------------------
void sonosApp::debugDraw()
{
	
	float scalef = camWidth / OUTPUT_WIDTH;
	int margin = 20;
	int spacerx = camWidth + margin;
	int spacery = camHeight + margin;
	
	ofPushMatrix();
	ofScale(scalef, scalef, 1.0);
	ofBackground(0, 0, 0);
	colorImg.draw(margin,margin);
	grayImage.draw(spacerx,margin);
	grayBg.draw(margin,spacery);
	grayDiff.draw(spacerx, spacery);
	
	// then draw the contours:
	ofFill();
	ofSetHexColor(0x333333);
	ofRect(spacerx, spacery,camWidth,camHeight);
	ofSetHexColor(0xffffff);
	
	// draw the whole contour finder
	contourFinder.draw(spacerx, spacery);
	
	ofPopMatrix();
	
	ofPushMatrix();
	
	drawDebugInterface(OUTPUT_WIDTH - 300, margin);
	
	ofPopMatrix();
	
}

//--------------------------------------------------------------
void sonosApp::background(int color)
{
	switch (color)
	{
		case 1:
			ofBackground(100, 100, 100);
			break;
		case 2:
			ofBackground(0, 0, 0);
			break;
		case 3:
			ofBackground(128,100,40);
			break;
		case 4:
			ofBackground(40, 150, 130, 50);
			break;
		default:
			break;
	}	
	
}
