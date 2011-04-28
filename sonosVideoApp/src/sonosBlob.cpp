/*
 *  sonosBlob.cpp
 *  sonosVideoApp
 *
 *  Created by tucano on 4/28/11.
 *  Copyright 2011 IFOM-FIRC. All rights reserved.
 *
 */

#include "sonosBlob.h"

void sonosBlob::setAvatar()
{
	avatar_x = centroid.x;
	avatar_y = boundingRect.y - (boundingRect.height / 2);
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
	ofSetHexColor(mycolor);
	float raggio = (boundingRect.height >= boundingRect.width ? boundingRect.height : boundingRect.width) / 5.0;
	ofCircle( avatar_x, avatar_y, raggio);
	ofPopStyle();
}