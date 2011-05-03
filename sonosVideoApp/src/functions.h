/*
 *  functions.h
 *  sonosVideoApp
 *
 *  Created by tucano on 4/28/11.
 *  Copyright 2011 IFOM-FIRC. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxCvBlob.h"

//--------------------------------------------------------------
// Random Functions
vector<float> randomScatter(float x, float y, int range);
vector<int> randomRgb();
std::string randomString(int length, bool letters, bool numbers, bool symbols);

//--------------------------------------------------------------
// sort helper functions
bool sortByCentroid(const ofxCvBlob& d1, const ofxCvBlob& d2);
