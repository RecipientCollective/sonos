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
	int	  mycolor;
	int   avatar_color;
	
	// costruttore (vuoto)
	sonosBlob() : ofxCvBlob() 
	{
		z = 0.0f;		
		mycolor = 0;
		avatar_color = 0;
	}
	
	/////////////////////////////////////////////////////////////////
	// copiatore da ofxCvBlob (a = ofCvBlob)
	// Questo metodo viene chiamata ad ogni update con contourFinder
	// quindi ogni frame ho un sonosBlob nuovo.
	/////////////////////////////////////////////////////////////////
	
	sonosBlob(const ofxCvBlob &other)
	{
		area 		 = other.area;
		length 		 = other.length;
		hole 		 = other.hole;
		nPts         = other.nPts;
		pts          = other.pts;
		centroid     = other.centroid;
		boundingRect = other.boundingRect;
		
		// set avatar position
		setAvatar();
	}
	
	void setAvatar();
	void drawAvatar();
	void drawPixels();
	void setBlobColor(int color);
	void drawRect();
	void circle();
	void rectangle();
};

#endif