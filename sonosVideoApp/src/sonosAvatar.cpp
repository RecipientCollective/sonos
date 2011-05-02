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

sonosAvatar::sonosAvatar(float ax, float ay, float ar)
{
	x = ax;
	y = ay;
	r = ar;
}

void sonosAvatar::setStyle(int mcolor)
{
	color = mcolor;
}

void sonosAvatar::draw()
{
	std::cerr << x << "," << y << "," << r << "." << std::endl;
	ofPushStyle();
	ofSetHexColor(color);
	ofCircle( x, y, r);
	ofPopStyle();
}