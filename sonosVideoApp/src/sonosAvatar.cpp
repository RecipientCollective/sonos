/*
 *  sonosAvatar.cpp
 *  sonosVideoApp
 *
 *  Created by tucano on 5/2/11.
 *  Copyright 2011 IFOM-FIRC. All rights reserved.
 *
 */

#include "sonosAvatar.h"

sonosAvatar::sonosAvatar()
{
}

sonosAvatar::sonosAvatar(ofPoint c, ofRectangle br)
{
	r = (br.height >= br.width ? br.height : br.width) / 5.0;
	y  = br.y - (br.height / 2);
	x = c.x;
}

void sonosAvatar::setStyle(int mcolor)
{
	color = mcolor;
}

void sonosAvatar::draw()
{
	ofPushStyle();
	ofSetHexColor(color);
	ofCircle( x, y, r);
	ofSetHexColor(0);
	// draw a black point at center of avatar
	ofCircle(x, y, 1.0);
	ofPopStyle();
}

void sonosAvatar::drawParticles()
{
	ofPushStyle();
	int i = 0;
	for (i=0; i<10; i++) {
		int mrandx = ofRandom(-30,30);
		int mrandy = ofRandom(-30,30);
		int mx = x + mrandx;
		int my = y + mrandy;
		int r = ofRandom(0,255);
		int g = ofRandom(0,255);
		int b = ofRandom(0,255);
		ofSetColor(r, g, b);
		ofCircle(mx, my, 4.0);
#ifdef DEBUG
		std::cerr << "Drawing particle at: " << mx << "," << my << "." << std::endl;
#endif
	}
	ofPopStyle();
}