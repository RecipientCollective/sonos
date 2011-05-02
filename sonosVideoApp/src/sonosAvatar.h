/*
 *  sonosAvatar.h
 *  sonosVideoApp
 *
 *  Created by tucano on 5/2/11.
 *  
 *  Un Avatr per ogni sonosBlob.
 *  
 */

#include "ofMain.h"

class sonosAvatar
{
public:
	float x;
	float y;
	float r;
	int color;
	
	sonosAvatar();
	sonosAvatar(ofPoint c, ofRectangle br);
	
	void setStyle(int color);
	void draw();
	void drawParticles();
};