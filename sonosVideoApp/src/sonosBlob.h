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


class sonosBlob : public ofxCvBlob 
{
public:
	
	double z;        // Z-POSITION
	float avatar_x;
	float avatar_y;
	int   avatar_space;
	int	  mycolor;
	
	// costruttore (vuoto)
	sonosBlob() : ofxCvBlob() 
	{
	}
	
	// copiatore da ofxCvBlob (a = ofCvBlob)
	sonosBlob(const ofxCvBlob &other)
	{
		area 		 = other.area;
		length 		 = other.length;
		hole 		 = other.hole;
		nPts         = other.nPts;
		pts          = other.pts;
		centroid     = other.centroid;
		boundingRect = other.boundingRect;
		
		// No info on Z when I copy the blob
		z            = 0.0;
		avatar_space = 100;
		
		// default color
		mycolor = 0;
		
		// avatar position
		setAvatar();
	}
	
	void setAvatar();
	void drawPixels();
	void setBlobColor(int color);
	void drawRect();
	void circle();
	void rectangle();
};

#endif