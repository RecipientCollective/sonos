/*
 *  functions.cpp
 *  sonosVideoApp
 *
 *  Created by tucano on 4/28/11.
 *  Copyright 2011 IFOM-FIRC. All rights reserved.
 *
 */

#include "functions.h"

// sort helper functions
bool sortByCentroid(const ofxCvBlob& d1, const ofxCvBlob& d2)
{
	return d1.centroid.x < d2.centroid.x;
}

vector<float> randomScatter(float x, float y, int range)
{
	vector<float> xy(2);
	int mrandx = ofRandom(-range,range);
	int mrandy = ofRandom(-range,range);
	xy[0] =( x + mrandx);
	xy[1] = y + mrandy;
	return(xy);	
}

vector<int> randomRgb()
{
	vector<int> rgb(3);
	for (int i=0; i<3; i++) {
		rgb[i] = ofRandom(0,255);
	}
	return(rgb);
	
}