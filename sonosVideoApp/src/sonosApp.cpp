#include "ofxPUI.h"
#include "sonosApp.h"

/*
 * SONOS APP INTERFACE (NOTA: ofxPUI.h DEVE essere incluso qui e solo qui. Quindi l'interfaccia va messa tutta qui.
 * 
 * Metodi "interni" di sonoApp vanno in sonoAppMethods.cpp
 *
 */
//========================================================================
// Vars needed by the PUI interface

//--------------------------------------------------------------
//  PUI objects and global toggle vars
//--------------------------------------------------------------
bool bLearnBackground;
bool bFullscreen;
bool bDebug;
bool bInterface;
bool bCircle;
bool bLine;
bool bBox;
bool bAvatar;
int	 blobMax;
int  Threshold;
int  contour_min;
int	 mtrx;
int	 mtry;
int	 BckColor;
float scale_x;
float scale_y;

void learnBackground ( puObject * ob ) 
{
	bLearnBackground = true;
}

void setMaxBlobs ( puObject * ob ) 
{
	blobMax = ob->getValue();
}

void setThreshold ( puObject * ob ) 
{
	Threshold = ob->getValue();
}

void setContour ( puObject * ob ) 
{
	contour_min = ob->getValue();
}

void toggleFullScreen ( puObject * ob )
{
	bFullscreen = !bFullscreen;
}

void toggleDebug ( puObject * ob )
{
	bDebug = !bDebug;
}

void toggleCircle ( puObject * ob )
{
	bCircle = !bCircle;
}

void toggleLine ( puObject * ob )
{
	bLine = !bLine;
}

void toggleAvatar ( puObject * ob )
{
	bAvatar	= !bAvatar;
}

void setTrx (puObject * ob)
{
	mtrx = ob->getIntegerValue();
}

void setTry (puObject * ob)
{
	mtry = ob->getIntegerValue();
}

void resetAction(puObject * ob)
{
	scale_x = 1.0;
	scale_y = 1.0;
	mtrx = 1;
	mtry = 1;
}

void setScale (puObject * ob)
{
	scale_x = scale_y = ob->getFloatValue();
}

void setBackground (puObject * ob)
{
	BckColor = ob->getIntegerValue();
}

char *button_box_entries [] = { "0", "1", "2","3", NULL } ;


//--------------------------------------------------------------
// OpenFramework METHODS
//--------------------------------------------------------------

//--------------------------------------------------------------
void sonosApp::setup()
{
	
	inputSetup();
	
	setDefaults();

	setupInterface();
	
	setupParticleSystem();
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
	setFullScreen(bFullscreen);
	
	if (bDebug) {
		debugDraw();
	} else {
		sonosDraw();
	}
	if (bInterface) {
		drawInterface(20, 20);
	}
}

//--------------------------------------------------------------
void sonosApp::exit()
{
	//magari c'� da chiudere la cam o i video da verificare;
	OF_EXIT_APP(0);
}

//--------------------------------------------------------------

//--------------------------------------------------------------
// GLUT key modifiers
//--------------------------------------------------------------

bool ofKeyAlt() {  
    return (glutGetModifiers() & GLUT_ACTIVE_ALT);  
}  

bool ofKeyShift() {  
    return (glutGetModifiers() & GLUT_ACTIVE_SHIFT);  
}  

bool ofKeyControl() {  
    return (glutGetModifiers() & GLUT_ACTIVE_CTRL);  
}

//--------------------------------------------------------------
void sonosApp::keyPressed(int key)
{	
	/* 
	 *  MAP KEY 
	 *  arrows to translate/scale (MAIUSC) 
	 *  h: hide interface 
	 *  f: fullscreen
	 *  , LiveVideo options
	 */
	
	switch (key) {
			
		case 'h':
			bInterface = !bInterface;
			bBox = !bBox;
			break;
		case 'f':
			bFullscreen = !bFullscreen;
			break;
			
		case OF_KEY_UP:
			if (ofKeyShift()) {
				scale_x+=0.01;
				scale_y+=0.01;
			} else {
				mtry--;
			}
			break;
		case OF_KEY_DOWN:
			if (ofKeyShift()) {
				scale_x-=0.01;
				scale_y-=0.01;
			} else {
				mtry++;
			}
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
			ofxpuKeyboard ( key, PU_DOWN ) ;
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

//--------------------------------------------------------------
//
//   ****************************
//   INTERNAL METHODS of sonosApp
//   ****************************
//   
//--------------------------------------------------------------

//--------------------------------------------------------------
//  SETUP and LOAD
//--------------------------------------------------------------

//--------------------------------------------------------------
void sonosApp::inputSetup()
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
}

//--------------------------------------------------------------
void sonosApp::setDefaults()
{
	// ALLOCATE IMAGES SIZES
	colorImg.allocate(camWidth, camHeight);
	grayImage.allocate(camWidth, camHeight);
	grayBg.allocate(camWidth,camHeight);
	grayDiff.allocate(camWidth,camHeight);
	
	// SETUP DEFAULT PARAMETERS
	bLearnBackground = true;
	Threshold = 50;
	ofSetCircleResolution(40); 	   //set resolution of circle
	ofEnableSmoothing();	
	ofSetFrameRate(60);
	
	//for smooth animation, set vertical sync if we can
	//ofSetVerticalSync(true);
	
	//colors setup
	BckColor=0;
	BlobColor=0xDD00CC;
	
	blobMax=2;
	contour_min = 350;
	scale_x = 1.0;
	scale_y = 1.0;
	mtrx = 1;
	mtry = 1;
	bInterface = true;
	bCircle = false;
	bDebug = false;
	bLine = false;
	bBox = true;
	bAvatar = false;
}

//--------------------------------------------------------------
void sonosApp::setupParticleSystem()
{
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
//  SETUP/DRAW INTERFACE 
//--------------------------------------------------------------

//--------------------------------------------------------------
void sonosApp::setupInterface()
{
	// helper vars for PUI interface
	int align_top = OUTPUT_HEIGHT - 200;
	int align_left= 20;
	int spacer_bt = 30;
	int slider_correction = 20;
	int column_spacer = 100;
	int slider_box = column_spacer * 3; // COLUMN 4
	int slider_spacer = 90;
	
	// PUI setup
	ofxpuInit () ;
	ofxpuSetDefaultStyle(PUSTYLE_BOXED);
	
	// COLUMN 1
	ofxpuButton *fullScreenBt= new ofxpuButton(align_left, align_top, "FullScreen");
	fullScreenBt->setCallback(toggleFullScreen);
	
	ofxpuButton *backGroundBt= new ofxpuButton(align_left, align_top + spacer_bt, "Background");
	backGroundBt->setCallback(learnBackground);
	
	ofxpuButton *resetBt= new ofxpuButton(align_left, align_top + spacer_bt*2, "ResetVideo");
	resetBt->setCallback(resetAction);
	
	ofxpuButton *debugBt= new ofxpuButton(align_left, align_top + spacer_bt*3, "DebugVideo");
	debugBt->setCallback(toggleDebug);
	
	// COLUMN 2
	ofxpuButton *circleBt= new ofxpuButton(align_left + column_spacer, align_top, "Circle");
	circleBt->setCallback(toggleCircle);
	
	ofxpuButton *lineBt= new ofxpuButton(align_left + column_spacer, align_top + spacer_bt, " Line ");
	lineBt->setCallback(toggleLine);
	
	ofxpuButton *avatarBt= new ofxpuButton(align_left + column_spacer, align_top + spacer_bt*2, "Avatar");
	avatarBt->setCallback(toggleAvatar);
	
	
	// COLUMN 3
	ofxpuaSpinBox * spinBlobs = new ofxpuaSpinBox (align_left + column_spacer * 2, align_top, 60, 30);
	spinBlobs->setLabel("maxBlobs");
	spinBlobs->setLabelPlace(OFXPUPLACE_BOTTOM_CENTERED);
	spinBlobs->setMaxValue ( 20 ) ;
	spinBlobs->setValue ( blobMax ) ;
	spinBlobs->setMinValue( 0 );
	spinBlobs->setCallback(setMaxBlobs);
	
	ofxpuButtonBox * backGroundBox = new ofxpuButtonBox ( align_left + column_spacer * 2, align_top + spacer_bt + 10, 60.0, 100.0, button_box_entries, TRUE ) ;
	backGroundBox->setLabel("BackGround");
	backGroundBox->setLabelPlace(PUPLACE_BOTTOM_CENTER);
	backGroundBox->setCallback(setBackground);
	
	// SLIDERS
	ofxpuaSliderWithInput * sliderThreshold= new ofxpuaSliderWithInput ( slider_box, align_top - slider_correction, 70, 150, FALSE ) ;
	sliderThreshold->setLabelPlace(PUPLACE_BOTTOM_CENTERED) ;
	sliderThreshold->setLabel ( "Threshold" ) ;
	sliderThreshold->setMaxValue ( 255 ) ;
	sliderThreshold->setValue ( Threshold ) ;
	sliderThreshold->setMinValue( 0 );
	sliderThreshold->setCallback(setThreshold);
	
	ofxpuaSliderWithInput * sliderContour= new ofxpuaSliderWithInput (slider_box + slider_spacer, align_top - slider_correction, 70, 150, FALSE ) ;
	sliderContour->setLabelPlace(PUPLACE_BOTTOM_CENTERED) ;
	sliderContour->setLabel ( "ContourMin" ) ;
	sliderContour->setMaxValue ( 10000 ) ;
	sliderContour->setValue ( contour_min ) ;
	sliderContour->setMinValue( 0 );
	sliderContour->setCallback(setContour);
	
	ofxpuaSliderWithInput *sliderTrx = new ofxpuaSliderWithInput(slider_box + slider_spacer*2, align_top - slider_correction, 70, 150, FALSE);
	sliderTrx->setLabelPlace(PUPLACE_BOTTOM_CENTERED);
	sliderTrx->setLabel ( "Translate X" );
	sliderTrx->setMinValue(-1000);
	sliderTrx->setMaxValue(+1000);
	sliderTrx->setValue(mtrx);
	sliderTrx->setCallback(setTrx);
	
	ofxpuaSliderWithInput *sliderTry = new ofxpuaSliderWithInput(slider_box + slider_spacer*3, align_top - slider_correction, 70, 150, FALSE);
	sliderTry->setLabelPlace(PUPLACE_BOTTOM_CENTERED);
	sliderTry->setLabel ( "Translate Y" );
	sliderTry->setMinValue(-1000);
	sliderTry->setMaxValue(+1000);
	sliderTry->setValue(mtry);
	sliderTry->setCallback(setTry);
	
	ofxpuaSliderWithInput *sliderScaleX = new ofxpuaSliderWithInput(slider_box + slider_spacer*4, align_top - slider_correction, 70, 150, FALSE);
	sliderScaleX->setLabelPlace(PUPLACE_BOTTOM_CENTERED);
	sliderScaleX->setLabel ( "Scale XY" );
	sliderScaleX->setMinValue(0.0);
	sliderScaleX->setMaxValue(5.0);
	sliderScaleX->setValue(scale_x);
	sliderScaleX->setCallback(setScale);
	
}

//--------------------------------------------------------------
void sonosApp::drawInterface(float x, float y)
{
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("INTERFACE (press: h to hide/show)", x, y);
	char reportStr[1024];
	char help[1024] = "fps: %f\nnum blobs found %i, MaxBlobs: %i\nThreshold %i\nContourMinSize: %i\ntranslate (%i, %i)\nscale (%.2f, %.2f)\nCamWidth: %i, CamHeight: %i";
	sprintf(reportStr, help , ofGetFrameRate(),contourFinder.nBlobs, blobMax, Threshold, contour_min, mtrx, mtry, scale_x, scale_y, camWidth, camHeight);
	ofDrawBitmapString(reportStr, x, y + 25);
	
	ofPushStyle();
	ofPushMatrix();
	
	EXACT_PIXEL_BEGIN
	
	ofNoFill();
	ofSetHexColor(0xff0000);
	
	EXACT_PIXEL_END	
	
	ofxpuDisplay ();
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
//  SONOS UPDATE/DRAW
//--------------------------------------------------------------

//--------------------------------------------------------------
void sonosApp::sonosUpdate()
{
	// 1. SET IMAGE from current pixels, move to greyImg, if learn background do it
#ifdef _USE_LIVE_VIDEO
	colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
#else
	colorImg.setFromPixels(vidPlayer.getPixels(), camWidth,camHeight);
#endif		
	grayImage = colorImg;
	if (bLearnBackground == true){
		grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
		bLearnBackground = false;
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
	}	
}

//--------------------------------------------------------------
void sonosApp::sonosDraw()
{	
	// background
	sonosApp::background(BckColor);
	
	ofPushMatrix();
	ofScale(scale_x, scale_y, 1.0);
	ofTranslate(mtrx, mtry, 1.0);
	
	if (bBox) {
		ofPushStyle();
		ofSetColor(255, 255, 255);
		ofNoFill();
		ofRect(0, 0, camWidth, camHeight);
		ofPopStyle();
	}
	
	// in draw we iterate in the map
	for(map<int, sonosBlob>::iterator i = sonosblobs.begin(); i != sonosblobs.end(); ++i)
	{
		sonosBlob curr_blob = i->second;
		curr_blob.setBlobColor(BlobColor);
		curr_blob.drawPixels();
		
		// IF interface: draw rectangle around 
		if(bInterface) curr_blob.drawRect();
		
		// set color by position (TEST)
		ofPushStyle();
		if (i->first == 0) {
			ofSetColor(255, 0, 0);
		} else if (i->first == 1) {
			ofSetColor(0, 255, 0);
		} else if (i->first == 2) {
			ofSetColor(0, 0, 255);
		} else {
			ofSetColor(100, 100, 150);
		}
		
		if (bCircle) curr_blob.circle();
		if (bLine) curr_blob.rectangle();
		ofPopStyle();
		
		// test AVATAR
		if (bAvatar) curr_blob.drawAvatar(particleSystem);
	}
	
	ofPopMatrix();
}

//--------------------------------------------------------------
//  DEBUG DRAW and HELPERS
//--------------------------------------------------------------

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
}

//--------------------------------------------------------------
void sonosApp::setFullScreen(bool full)
{
	if (full) {
		ofSetFullscreen(true);
	} else {
		ofSetWindowShape(OUTPUT_WIDTH,OUTPUT_HEIGHT);
		ofSetFullscreen(false);
	}
}

//--------------------------------------------------------------
void sonosApp::background(int color)
{
	switch (color)
	{
		case 0:
			ofBackground(100, 100, 100);
			break;
		case 1:
			ofBackground(0, 0, 0);
			break;
		case 2:
			ofBackground(128,100,40);
			break;
		case 3:
			ofBackground(40, 150, 130, 50);
			break;
		default:
			break;
	}	
	
}
