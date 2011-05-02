/*
 *  sonosAvatar.h
 *  sonosVideoApp
 *
 *  Created by tucano on 5/2/11.
 *  
 *  Un Avatr per ogni sonosBlob.
 *  
 */

#include "ofxOpenCv.h"

class sonosAvatar
{
public:
	float x;
	float y;
	float r;
	int color;
	
	sonosAvatar();
	sonosAvatar(float ax, float ay, float ar);
	
	void setStyle(int color);
	void draw();
	
};