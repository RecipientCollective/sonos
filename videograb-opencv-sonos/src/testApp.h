#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenCv.h"


//#define _USE_LIVE_VIDEO	

#define OUTPUT_HEIGHT 768
#define OUTPUT_WIDTH 1024

class testApp : public ofBaseApp
{

	public:

		void setup();
		void update();
		void draw();
		void exit();
	
		void drawPointCloud();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		ofxCvColorImage		colorImg;
		ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;

		ofxCvContourFinder 	contourFinder;
		
		bool				bThreshWithOpenCV;
		bool				bFullscreen;

		int					colorz;
		int					blobMax;

#ifdef _USE_LIVE_VIDEO
	ofVideoGrabber 		vidGrabber;
#else
	ofVideoPlayer* 		vidPlayer;
#endif
	//unsigned char * 	videoInverted;
	//ofTexture			videoTexture;
	int 				camWidth;
	int 				camHeight;
	int 				Threshold;
	
	float scale_x;
	float scale_y;
	float mtrx;
	float mtry;
	bool interface;
	bool circle;
	bool debug;
	bool rectangle;
	bool videosurce;
	bool bLearnBakground;
	int contour_min;
	
	bool openVideo;
	
	int openFile(string& URL);
};

#endif
