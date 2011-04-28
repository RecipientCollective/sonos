#include "sonosApp.h"

//--------------------------------------------------------------
void sonosApp::setup()
{
	
#ifdef _USE_LIVE_VIDEO
	camWidth = 640;
	camHeight = 480;
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth, camHeight, false);
#else
	// FILENAME
	filename = "videos/retro-ir.mov";
	
	//check if file exists
	bool bFileThere = false;
	fstream fin;
	string fileNameInOF = ofToDataPath(filename, false); // since OF files are in the data directory, we need to do this
	fin.open(fileNameInOF.c_str(),ios::in);
	if ( fin.is_open() ) {
#ifdef DEBUG
		cerr << "Founded file " << fileNameInOF << endl;
#endif
		bFileThere =true;
	}
	fin.close();
	
	if (bFileThere) {
		vidPlayer.loadMovie(filename);
		vidPlayer.play();
		camWidth = vidPlayer.getWidth();
		camHeight = vidPlayer.getHeight();
	} else {
#ifdef DEBUG		
		cerr << "File " << fileNameInOF << " is not here!" << endl;
#endif
		sonosApp:exit();
	}
#endif

#ifdef DEBUG		
	cerr << "Input size: width =" << camWidth << " height = " << camHeight << endl;
#endif
	
	// ALLOCATE IMAGES SIZES
	colorImg.allocate(camWidth, camHeight);
	grayImage.allocate(camWidth, camHeight);
	grayBg.allocate(camWidth,camHeight);
	grayDiff.allocate(camWidth,camHeight);
	
	// SETUP DEFAULT PARAMETERS
	bLearnBakground = true;
	Threshold = 50;
	ofSetCircleResolution(40); 	   //set resolution of circle
	ofEnableSmoothing();	
	ofSetFrameRate(60);
	
	//for smooth animation, set vertical sync if we can
	//ofSetVerticalSync(true);
	
	//colors setup
	BckColor=1;
	BlobColor=0xDD00CC;
	
	blobMax=2;
	contour_min = 350;
	scale_x = 1.0;
	scale_y = 1.0;
	mtrx = 1;
	mtry = 1;
	interface = true;
	circle = false;
	debug = false;
	rectangle = false;
	
}

//--------------------------------------------------------------
void sonosApp::update()
{
	
	// IS A NEW FRAME?
	bool bNewFrame = false;
#ifdef _USE_LIVE_VIDEO
	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();
#else
	vidPlayer.idleMovie();
	bNewFrame = vidPlayer.isFrameNew();
#endif
	
	if (bNewFrame){
		// 1. SET IMAGE from current pixels, move to greyImg, if learn background do it
#ifdef _USE_LIVE_VIDEO
		colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
#else
		colorImg.setFromPixels(vidPlayer.getPixels(), camWidth,camHeight);
#endif		
		grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}
		
		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(Threshold);

		//flag updated the cv image
		grayImage.flagImageChanged();
		grayDiff.flagImageChanged();
		
		/* COUNTOUR FINDER FUNCTION
		 
		ofxCvContourFinder::findContours( 
		 ofxCvGrayscaleImage&  input,
		 int minArea,
		 int maxArea,
		 int nConsidered,
		 bool bFindHoles,
		 bool bUseApproximation
		)
		 
		find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		also, find holes is set to true so we will get interior contours as well...  */
		contourFinder.findContours(grayDiff, contour_min, (camWidth*camHeight)/3, blobMax, false, true);
			
		// sort by centroid
		std::sort(contourFinder.blobs.begin(),contourFinder.blobs.end(), sortByCentroid);
		
		// clean sonosblob map
		sonosblobs.clear();
		
		// copy the blobs to sonosBlobs map
		for(int i = 0; i < contourFinder.blobs.size(); i++) {
			
			sonosBlob myblob = contourFinder.blobs[i];
			sonosblobs.insert(std::pair<int, sonosBlob>(i,myblob));
			// myblob ha area quindi e' figlio di ofxCvBlob
			// std::cout << "AREA: " << myblob.area << std::endl;
			// myblob ha anche Z
			// std::cout << "Z: " << myblob.z << std::endl;
		}
	}
}

//--------------------------------------------------------------
void sonosApp::draw()
{
	if (debug) {
		debugDraw();
	} else {
		// background
		sonosApp::background(BckColor);
	}
	
	if (interface) {
		drawInterface(20, 20);
	}
}

//--------------------------------------------------------------
void sonosApp::setFullScreen(bool full)
{
	if (full) {
		ofSetFullscreen(true);
	} else {
		ofSetWindowShape(OUTPUT_WIDTH,OUTPUT_HEIGHT);
		ofSetFullscreen(false);
		// figure out how to put the window in the center:
		int screenW = ofGetScreenWidth();
		int screenH = ofGetScreenHeight();
		ofSetWindowPosition(screenW/2-OUTPUT_WIDTH/2, screenH/2-OUTPUT_HEIGHT/2);
	}
}

//--------------------------------------------------------------
void sonosApp::exit()
{
	//magari c' da chiudere la cam o i video da verificare;
	OF_EXIT_APP(0);
}

void sonosApp::drawInterface(float x, float y)
{
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("INTERFACE (press: h to hide)", x, y);
	char reportStr[1024];
	char help[1024] = "fps: %f\nnum blobs found %i\nMaxBlobs: %i (< >)\nThreshold %i (+ -)\nSPACEBAR: learn background\nContourMinSize: %i (t y)\nf: fullscreen\nARROWS: translate (%i, %i)\n[a : z] scale (%.2f, %.2f)\nr : reset scale and translate\nTESTS: colors (1-n), circle (c), rectangle (r)";
	sprintf(reportStr, help , ofGetFrameRate(),contourFinder.nBlobs, blobMax, Threshold, contour_min, mtrx, mtry, scale_x, scale_y);
	ofDrawBitmapString(reportStr, x, y + 20);
}

//--------------------------------------------------------------
void sonosApp::debugDraw()
{

	float scalef = camWidth / OUTPUT_WIDTH;
	int margin = 20;
	int spacerx = camWidth + margin;
	int spacery = camHeight + margin;
		
	ofPushMatrix();
	ofScale(scalef, scalef, 1.0);
	ofBackground(0, 0, 0);
	colorImg.draw(margin,margin);
	grayImage.draw(spacerx,margin);
	grayBg.draw(margin,spacery);
	grayDiff.draw(spacerx, spacery);
	
	// then draw the contours:
	ofFill();
	ofSetHexColor(0x333333);
	ofRect(spacerx, spacery,camWidth,camHeight);
	ofSetHexColor(0xffffff);
	
	// draw the whole contour finder
	contourFinder.draw(spacerx, spacery);
	
	ofPopMatrix();
	
	ofPushMatrix();
	
	drawDebugInterface(OUTPUT_WIDTH - 300, margin);
	
	ofPopMatrix();
	
}

void sonosApp::drawDebugInterface(float x, float y)
{
	char reportStr[1024];
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("DEBUG INTERFACE", x, y);
	sprintf(reportStr, "CamWidth: %i, CamHeight: %i\nfps: %f\nnum blobs found %i", camWidth, camHeight, ofGetFrameRate(),contourFinder.nBlobs);
	ofDrawBitmapString(reportStr, x, y + 20);
}

//--------------------------------------------------------------
void sonosApp::background(int color)
{
	switch (color)
	{
		case 1:
			ofBackground(100, 100, 100);
			break;
			
		case 2:
			ofBackground(0, 0, 0);
			break;
		default:
			break;
	}	
	
}

//--------------------------------------------------------------
void sonosApp::keyPressed(int key)
{
	switch (key) {
		case '1':
			BckColor=1;
			break;
		case '2':
			BckColor=2;
			break;
		case 'd':
			debug = !debug;
			break;
		case 'h':
			interface = !interface;
			break;
		case '+':
			Threshold ++;
			if (Threshold > 255) Threshold = 255;
			break;
		case '-':
			Threshold --;
			if (Threshold < 0) Threshold = 0;
			break;
		case ' ':
			bLearnBakground = true;
			break;
		case '>':
			blobMax ++;
			break;
		case '<':	
			blobMax --;
			if (blobMax < 0) blobMax = 0;
			break;
		case 'f':
			bFullscreen = !bFullscreen;
			sonosApp::setFullScreen(bFullscreen);
			break;
		case 'a':
			scale_x+=0.01;
			scale_y+=0.01;
			break;
		case 'z':
			scale_x-=0.01;
			scale_y-=0.01;
			break;
		case 'r':
			scale_x = 1.0;
			scale_y = 1.0;
			mtrx = 1.0;
			mtry = 1.0;
			break;
		case 't':
			contour_min--;
			break;
		case 'y':
			contour_min++;
			break;
			
		// TEST
		case 'c':
			circle = !circle;
			break;
		case 'q':
			rectangle = !rectangle;
			break;
			
		// arrows
		case OF_KEY_UP:
			mtry--;
			break;
		case OF_KEY_DOWN:
			mtry++;	
			break;
		case OF_KEY_LEFT:
			mtrx--;		
			break;
		case OF_KEY_RIGHT:
			mtrx++;
			break;
#ifdef _USE_LIVE_VIDEO
		case ',':
			vidGrabber.videoSettings();
			break;
#endif			
		default:
			break;
	}
	
}

//--------------------------------------------------------------
void sonosApp::keyReleased(int key)
{
	
}

//--------------------------------------------------------------
void sonosApp::mouseMoved(int x, int y )
{
	
}

//--------------------------------------------------------------
void sonosApp::mouseDragged(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void sonosApp::mousePressed(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void sonosApp::mouseReleased(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void sonosApp::windowResized(int w, int h)
{
	
}

//--------------------------------------------------------------
void sonosApp::gotMessage(ofMessage msg)
{
	
}

//--------------------------------------------------------------
void sonosApp::dragEvent(ofDragInfo dragInfo)
{ 
	
}