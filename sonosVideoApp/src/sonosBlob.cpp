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

void sonosBlob::drawAvatar(ParticleSystem p)
{
//	ofPushStyle();
//	ofSetHexColor(mycolor);
//	float raggio = (boundingRect.height >= boundingRect.width ? boundingRect.height : boundingRect.width) / 5.0;
//	ofCircle( avatar_x, avatar_y, raggio);
//	ofPopStyle();
	
	// testing fluids
	ofPushStyle();
	Vec2f pos = Vec2f (avatar_x,avatar_y);
	Vec2f vel = Vec2f (10,10);
//		int index = fluidSolver.getIndexForPos(pos);
//		Color drawColor( CM_HSV, ( getElapsedFrames() % 360 ) / 360.0f, 1, 1 );
//		fluidSolver.addColorAtIndex(index, drawColor * colorMult);
	
	p.addParticles(pos , 10 );
//	fluidSolver.addForceAtIndex(index, vel * velocityMult);
//		glColor3f(1, 1, 1);
//		fluidDrawer.draw(0, 0, camWidth, camHeight);
	p.updateAndDraw( true );	
	ofPopStyle();
}