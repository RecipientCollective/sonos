/*
 *  functions.cpp
 *  sonosVideoApp
 *
 *  Created by tucano on 4/28/11.
 *  Copyright 2011 IFOM-FIRC. All rights reserved.
 *
 */

#include "ofxCvBlob.h"

// sort helper functions
bool sortByCentroid(const ofxCvBlob& d1, const ofxCvBlob& d2)
{
	return d1.centroid.x < d2.centroid.x;
}