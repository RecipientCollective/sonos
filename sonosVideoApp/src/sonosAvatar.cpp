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

void sonosAvatar::setPosition(ofPoint c, ofRectangle br)
{
	r = (br.height >= br.width ? br.height : br.width) / 5.0;
	y = br.y - (br.height / 2);
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
		vector<float> pos = randomScatter(x, y, 100);
		vector<int> rgb = randomRgb();
		ofSetColor(rgb[0], rgb[1], rgb[2]);
		ofCircle(pos[0], pos[1], 4.0);
#ifdef DEBUG
		std::cerr << "Drawing particle at: " << pos[0] << "," << pos[1] << "." << std::endl;
#endif
	}
	ofPopStyle();
}