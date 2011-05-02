/*
 *  sonosBlob.cpp
 *  sonosVideoApp
 *
 *  Created by tucano on 4/28/11.
 *
 */

#include "sonosBlob.h"

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
	
	// init avatar
	//avatar = new sonosAvatar(centroid.x, centroid.y);
	
	setAvatar();
}

void sonosBlob::setAvatar()
{
	avatar_x = centroid.x;
	avatar_y = boundingRect.y - (boundingRect.height / 2);
	// FIXME avatar colorb
	avatar_color = mycolor;
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

void sonosBlob::drawAvatar()
{
	ofPushStyle();
	ofSetHexColor(avatar_color);
	float raggio = (boundingRect.height >= boundingRect.width ? boundingRect.height : boundingRect.width) / 5.0;
	ofCircle( avatar_x, avatar_y, raggio);
	ofPopStyle();
}