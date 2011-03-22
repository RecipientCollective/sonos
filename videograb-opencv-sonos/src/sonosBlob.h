/*
 *  sonosBlob.h
 *  videograb
 *
 *  Created by drambald on 3/22/11.
 *  Copyright 2011 recipient.cc . All rights reserved.
 *
 */

#ifndef _SONOS_BLOB
#define _SONOS_BLOB

#include "ofxOpenCv.h"

class sonosBlob : public ofxCvBlob 
{
public:
	
	double z;        // Z-POSITION
	
	// costruttore (vuoto)
	sonosBlob() : ofxCvBlob() 
	{
		z = 0.0;
	}

	// copiatore da ofxCvBlob (a = ofCvBlob)
	sonosBlob(const ofxCvBlob &other)
	{
		area 		 = other.area;
		length 		 = other.length;
		hole 		 = other.hole;
		nPts         = other.nPts;
		centroid     = other.centroid;
		boundingRect = other.boundingRect;
		
		// No info on Z when I copy the blob
		z            = 0.0;
	}
	
};

#endif