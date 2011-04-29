#include "ofxPUI.h"
#include "sonosApp.h"

/*
 * SONOS APP INTERFACE (NOTA: ofxPUI.h DEVE essere incluso qui e solo qui. Quindi l'interfaccia va messa tutta qui.
 * 
 * Metodi "interni" di sonoApp vanno in sonoAppMethods.cpp
 *
 */

//--------------------------------------------------------------
//  PUI objects
//--------------------------------------------------------------

void button_cb ( puObject * ) 
{
	fprintf ( stderr, "Hello World.\n" ) ;
}


//--------------------------------------------------------------
//  DRAW INTERFACE METHDOS
//--------------------------------------------------------------

//--------------------------------------------------------------
void sonosApp::drawInterface(float x, float y)
{
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("INTERFACE (press: h to hide)", x, y);
	char reportStr[1024];
	char help[1024] = "fps: %f\nnum blobs found %i\n(< >) MaxBlobs: %i\n(+ -) Threshold %i\nSPACEBAR: learn background\n(t y) ContourMinSize: %i\nf: fullscreen\nARROWS: translate (%i, %i)\n[a : z] scale (%.2f, %.2f)\nr : reset scale and translate\nb: hide/show box\n\nTESTS: colors (1-n), avatar (p), circle (c), rectangle (r)";
	sprintf(reportStr, help , ofGetFrameRate(),contourFinder.nBlobs, blobMax, Threshold, contour_min, mtrx, mtry, scale_x, scale_y);
	ofDrawBitmapString(reportStr, x, y + 20);
	
	ofPushStyle();
	ofPushMatrix();
	// test draw PUI interface
	EXACT_PIXEL_BEGIN
	
	ofNoFill();
	ofSetHexColor(0xff0000);
	
	EXACT_PIXEL_END
	
	
	ofxpuDisplay ();
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void sonosApp::drawDebugInterface(float x, float y)
{
	char reportStr[1024];
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("DEBUG INTERFACE", x, y);
	sprintf(reportStr, "CamWidth: %i, CamHeight: %i\nfps: %f\nnum blobs found %i", camWidth, camHeight, ofGetFrameRate(),contourFinder.nBlobs);
	ofDrawBitmapString(reportStr, x, y + 20);
}



//--------------------------------------------------------------
// OF METHODS
//--------------------------------------------------------------

//--------------------------------------------------------------
void sonosApp::setup()
{
	// PUI setup
	ofxpuInit () ;
	ofxpuSetDefaultStyle(PUSTYLE_BOXED);
	ofxpuButton *button1 = new ofxpuButton(10.0, 400.0, "Button 1");
	button1->setCallback(button_cb);
	
	inputSetup();
	
	setDefaults();
	
	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup( &fluidSolver );
	particleSystem.setFluidSolver( &fluidSolver );
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);
	windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cos I don't think it is called)	
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
		fluidSolver.update();
		sonosUpdate();
	}
}

//--------------------------------------------------------------
void sonosApp::draw()
{
	if (debug) {
		debugDraw();
	} else {
		sonosDraw();
	}
	
	if (interface) {
		drawInterface(20, 20);
	

	
	}
}

//--------------------------------------------------------------
void sonosApp::exit()
{
	//magari c' da chiudere la cam o i video da verificare;
	OF_EXIT_APP(0);
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
		case '3':
			BckColor=3;
			break;
		case '4':
			BckColor=4;
			break;
		case 'd':
			debug = !debug;
			break;
		case 'h':
			interface = !interface;
			box = !box;
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
		case 'p':
			avatar = !avatar;
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
	ofxpuMouse ( x, y );
}

//--------------------------------------------------------------
void sonosApp::mouseDragged(int x, int y, int button)
{
	ofxpuMouse ( x, y );
}

//--------------------------------------------------------------
void sonosApp::mousePressed(int x, int y, int button)
{
	ofxpuMouse ( button, GLUT_DOWN, x, y );
}

//--------------------------------------------------------------
void sonosApp::mouseReleased(int x, int y, int button)
{
	ofxpuMouse ( button, GLUT_UP, x, y );		
}

//--------------------------------------------------------------
void sonosApp::windowResized(int w, int h)
{
	particleSystem.setWindowSize( Vec2f( w, h ) );
}

//--------------------------------------------------------------
void sonosApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void sonosApp::dragEvent(ofDragInfo dragInfo)
{ 
	
}