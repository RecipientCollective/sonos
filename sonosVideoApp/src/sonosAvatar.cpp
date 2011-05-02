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
#ifdef DEBUG
	std::cerr << x << "," << y << "," << r << "." << std::endl;
#endif
	ofPushStyle();
	ofSetHexColor(color);
	ofCircle( x, y, r);
	ofPopStyle();
}