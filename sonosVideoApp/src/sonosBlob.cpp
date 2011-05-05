/*
 *  sonosBlob.cpp
 *  sonosVideoApp
 *
 *  Created by tucano on 4/28/11.
 *
 */

#include "sonosBlob.h"

// COSTRUTTORE
sonosBlob::sonosBlob() : ofxCvBlob()
{
	z = 0.0f;		
	mycolor = 0;
	avatar_color = 0;
	code = "";
}


/////////////////////////////////////////////////////////////////
// copiatore da ofxCvBlob (a = ofCvBlob)
// Questo metodo viene chiamata ad ogni update con contourFinder
// quindi ogni frame ho un sonosBlob nuovo.
/////////////////////////////////////////////////////////////////

sonosBlob::sonosBlob(const ofxCvBlob &other)
{
	area 		 = other.area;
	length 		 = other.length;
	hole 		 = other.hole;
	nPts         = other.nPts;
	pts          = other.pts;
	centroid     = other.centroid;
	boundingRect = other.boundingRect;
	
	// init avatar position
	avatar.setPosition(centroid, boundingRect);
	// prove of principle object persistency
	code = randomString(10, true, false, false);
	// on start the blob is set as updated
	updated = true;
	
//#ifdef DEBUG
//	std::cerr << "generated code for sonosBlob: " << code << std::endl;
//#endif
}

void sonosBlob::update(const ofxCvBlob blob)
{
	area 		 = blob.area;
	length 		 = blob.length;
	hole 		 = blob.hole;
	nPts         = blob.nPts;
	pts          = blob.pts;
	centroid     = blob.centroid;
	boundingRect = blob.boundingRect;
	
	updated = true;
	
	// init avatar position
	avatar.setPosition(centroid, boundingRect);
}

void sonosBlob::drawPixels()
{
	ofPushStyle();
	ofSetHexColor(mycolor);
	ofBeginShape();
	for( int j=0; j<nPts; j++ ) {
		ofVertex( pts[j].x, pts[j].y );
	}
	ofEndShape();
	ofPopStyle();
}

void sonosBlob::setBlobColor(int color)
{
	mycolor = color;
}

void sonosBlob::drawRect()
{
	ofPushStyle();
	ofNoFill();
	ofSetColor(255,0,0);
	ofRect(boundingRect.x,boundingRect.y,boundingRect.width,boundingRect.height);
	ofPopStyle();	
}

void sonosBlob::circle()
{
	float raggio = (boundingRect.height >= boundingRect.width ? boundingRect.height : boundingRect.width) / 1.5;
	ofCircle( centroid.x, centroid.y, raggio);
}

void sonosBlob::rectangle()
{
	ofRect(boundingRect.x,0,boundingRect.width, 480);
}