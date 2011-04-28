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
	avatar_y = boundingRect.x - avatar_space;
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