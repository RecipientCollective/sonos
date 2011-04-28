#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

//#define _USE_LIVE_VIDEO

#define OUTPUT_HEIGHT 768
#define OUTPUT_WIDTH 1024

class sonosApp : public ofBaseApp{
	
public:
	// APP methods
	void setup();
	void update();
	void draw();
	void exit();
	
	// VideoGrabber/Player
#ifdef _USE_LIVE_VIDEO
	ofVideoGrabber 		vidGrabber;
#else
	ofVideoPlayer 		vidPlayer;
#endif
	
	// openCV stuffs
	ofxCvColorImage			colorImg;
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	ofxCvContourFinder		contourFinder;
	
	// openFrameworks stuffs
	
	
	// Our methods
	void drawPointCloud();
	void background(int color);
	bool sortByCentroid(const ofxCvBlob& d1, const ofxCvBlob& d2);
	
	// Our vars
	std::string filename;
	int 		camWidth;
	int 		camHeight;
	int 		Threshold;
	bool		bThreshWithOpenCV;
	bool		bFullscreen;
	bool        bLearnBakground;
	int			blobMax;
	int			BlobColor;
	int			BckColor;
	int			colorz;
	int         contour_min;
	float       scale_x;
	float       scale_y;
	float		mtrx;
	float		mtry;
	bool        interface;
	bool        circle;
	bool        debug;
	bool        rectangle;
	
	// Events
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
