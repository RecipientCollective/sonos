/*
 *  sonosAvatar.h
 *  sonosVideoApp
 *
 *  Created by tucano on 5/2/11.
 *  
 *  Un Avatr per ogni sonosBlob.
 *  
 */

#ifndef _SONOS_AVATAR
#define _SONOS_AVATAR

#include "MSAPhysics2D.h"
#include "ofMain.h"
#include "functions.h"

using namespace MSA;

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

	// Physics
	// Physics::Particle2D body;
	
};

#endif