/*
 *  sonosBlob.h
 *  sonosVideoApp
 *
 *  Created by tucano on 4/28/11.
 *  Copyright 2011 IFOM-FIRC. All rights reserved.
 *
 */

#ifndef _SONOS_BLOB
#define _SONOS_BLOB

#include "ofxOpenCv.h"
#include "MSAPhysics2D.h"
#include "functions.h"
#include "sonosAvatar.h"

class sonosBlob : public ofxCvBlob 
{
public:
	
	double z;        // Z-POSITION
	float avatar_x;
	float avatar_y;
	int	  mycolor;
	int   avatar_color;
	string  code;
	
	// AVATAR
	sonosAvatar avatar;
	
	// costruttori
	sonosBlob();
	sonosBlob(const ofxCvBlob &other);

	void drawPixels();
	void setBlobColor(int color);
	void drawRect();
	void circle();
	void rectangle();
	void update(const ofxCvBlob blob);
};

#endif