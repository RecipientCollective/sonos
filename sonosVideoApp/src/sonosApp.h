#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

#define OUTPUT_HEIGHT 768
#define OUTPUT_WIDTH 1024

class sonosApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		
		void drawPointCloud();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
