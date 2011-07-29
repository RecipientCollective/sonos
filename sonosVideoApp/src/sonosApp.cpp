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
bool bToogleFullScreen;
bool bDebug;
bool bInterface;
bool bCircle;
bool bLine;
bool bBox;
bool bAvatar;
bool bPause;
bool bDrawImage;
int	 blobMax;
int  Threshold;
int  contour_min;
int	 mtrx;
int	 mtry;
int	 BckColor;
int	 DrawImageCounter;
float scale_x;
float scale_y;
int				current_msg_string;
string		msg_strings[NUM_MSG_STRINGS];
float			timers[NUM_MSG_STRINGS];

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
	bToogleFullScreen = true;
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

	setupPhysicsWorld();
	
	setupInterface();
	
	setupOsc();
	
#ifdef DEBUG
	std::cerr << "END SETUP" << std::endl;
#endif
}

//--------------------------------------------------------------
void sonosApp::update()
{
	OscUpdate();
	
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
		sonosUpdate();
		physics.update();
		
	}
}

//--------------------------------------------------------------
void sonosApp::draw()
{
	setFullScreen();
	
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
	//magari c' da chiudere la cam o i video da verificare;
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
			bToogleFullScreen = true;
			break;
#ifndef _USE_LIVE_VIDEO			
		case 'p':
			bPause = !bPause;
			vidPlayer.setPaused(bPause);
			break;
#endif	
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
		case ' ':
			bDrawImage=true;
			break;
        
        // TEST particles
        case 'z':
            makeParticle(1,0);
            break;
        case 'x':
            makeParticle(2,0);            
            break;
        case 'c':
            makeParticle(3,0);            
            break;
        case 'v':
            makeParticle(4,0);            
            break;
        case 'q':
            makeParticle(1,1);
            break;
        case 'w':
            makeParticle(2,1);            
            break;
        case 'e':
            makeParticle(3,1);            
            break;
        case 'r':
            makeParticle(4,1);            
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
	//particleSystem.setWindowSize( Vec2f( w, h ) );
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
	
    lettering1.loadImage("images/lettering_1.png");
    lettering2.loadImage("images/lettering_2.png");
    lettering3.loadImage("images/lettering_3.png");
    lettering4.loadImage("images/lettering_4.png");
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
	
	// random seed with Clock
	ofSeedRandom();
	
	//colors setup
	BckColor=0;
	BlobColor=0xDD00CC;
	
	blobMax=2;
	contour_min = 350;
	scale_x = 1.0;
	scale_y = 1.0;
	mtrx = 1;
	mtry = 1;
	DrawImageCounter = 0;
	bInterface = true;
	bCircle = false;
	bDebug = false;
	bLine = false;
	bBox = true;
	bAvatar = false;
	bDrawImage = false;
}

//--------------------------------------------------------------
void sonosApp::setupPhysicsWorld()
{
#ifdef DEBUG
	physics.verbose = true;			// dump activity to log
	std::cerr << "Dumping Physics INFO to log" << std::endl;
#endif
	// gravita'
	physics.setGravity(Vec2f(0,GRAVITY));
	
	// set world dimensions, not essential, but speeds up collision
	physics.setWorldSize(Vec2f(0.0,0.0), Vec2f(camWidth, camHeight));
    
	/* STRANGE SETS TODO FIXME */
	physics.setSectorCount(SECTOR_COUNT);
    physics.setDrag(0.97f);
	physics.setDrag(1);		// FIXTHIS
	physics.enableCollision();
	
	// clear all particles and springs etc
	physics.clear();
	
}

//--------------------------------------------------------------
void sonosApp::setupOsc()
{
	// listen on the given port
#ifdef DEBUG
    std::cerr << "listening for osc messages on port " << PORT << std::endl;
#endif
	receiver.setup( PORT );
	current_msg_string = 0;
}

//--------------------------------------------------------------

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
	char help[1024] = "fps: %f\nnum blobs found %i, MaxBlobs: %i\nThreshold %i\nContourMinSize: %i\ntranslate (%i, %i)\nscale (%.2f, %.2f)\nCamWidth: %i, CamHeight: %i\nParticles on screen: %i";
	sprintf(reportStr, help , ofGetFrameRate(),contourFinder.nBlobs, blobMax, Threshold, contour_min, mtrx, mtry, scale_x, scale_y, camWidth, camHeight,physics.numberOfParticles());
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

	/* 
	 * UPDATE with OBJECT PERSISTENCY
	 * 
	 * BITMASK (defined in sonosApp.h): (http://www.dylanleigh.net/notes/c-cpp-tricks.html#Using_"Bitflags")
	 *  1 SONOSEMPTY
	 *  2 BLOBSEMPTY
	 *  4 MORESONOS
	 *  8 MOREBLOBS
	 * 16 EQUALSIZE
	 *  
	 * Es: BOTH EMPTY = 3, SONOS EMPTY but BLOBS on SCREEN = 9, SONOS NOT EMPTY BUT no blobs on SCREEN = 4
	 *
	 */
	
	// reset all flags
	flags = 0;
	
	if (sonosblobs.size() == 0) flags |= SONOSEMPTY;
	if (contourFinder.blobs.size() == 0) flags |= BLOBSEMPTY;
	if (contourFinder.blobs.size() > sonosblobs.size()) flags |= MOREBLOBS;
	if (contourFinder.blobs.size() < sonosblobs.size()) flags |= MORESONOS;
	if (contourFinder.blobs.size() == sonosblobs.size()) flags |= EQUALSIZE;
	
	// CASO 1: niente sullo schermo
	if ((flags & (SONOSEMPTY | BLOBSEMPTY)) == (SONOSEMPTY | BLOBSEMPTY)) {
		
		// niente da fare ...
		
	// CASO 2: sonosblobs vuoto ma blobs on screen
	} else if ((flags & (SONOSEMPTY | MOREBLOBS)) == (SONOSEMPTY | MOREBLOBS)) {
#ifdef DEBUG
		std::cerr << "Event: BLOBS ON SCREEN, SONOS EMPTY" << " FLAGS: ";
		printf("%d", flags);
		std::cerr << std::endl;
#endif			
		sonosBlobsInsert();
		
	// CASO 3: sonosblobs pieno e blobs on screen equal size: remapping
	} else if ((flags & EQUALSIZE) == EQUALSIZE) {
		
		sonosBlobsUpdate();
				
	// CASO 4: sonosblobs piu' piccolo di blobs on screen. Qualcosa e' apparito
	} else if ((flags & MOREBLOBS) == MOREBLOBS) {

#ifdef DEBUG		
		std::cerr << "Event: MORE BLOBS ON SCREEN than blobs in sonos." << " FLAGS: ";
		printf("%d", flags);
		std::cerr << std::endl;
#endif
		
		sonosBlobsUpdate();
	
		sonosBlobsInsert();
						

	// CASO 5: sonosblobs piu' grande di blob on screen. Qualcosa e' sparito
	} else if ((flags & MORESONOS) == MORESONOS) {
#ifdef DEBUG		
		std::cerr << "Event: LESS BLOBS ON SCREEN than blobs in sonos."  << " FLAGS: ";
		printf("%d", flags);
		std::cerr << std::endl;
#endif		
		// FIXME OPTIMIZE?
		// per ora: un giro per settare tutti NOT updated
		for(map<string, sonosBlob>::iterator it = sonosblobs.begin(); it != sonosblobs.end(); ++it)
		{
			it->second.updated = false;
		}
		
		// update ...
		sonosBlobsUpdate();

		// cancella i NOT updated
		for (map<string, sonosBlob>::iterator it = sonosblobs.begin(); it != sonosblobs.end();) {
			if (!it->second.updated) {
#ifdef DEBUG
                std::cerr << "Erasing blob: " << it->second.code << std::endl;
#endif
                it->second.clean();
				sonosblobs.erase(it);
				it++;
			} else {
				it++;
			}
		}
	}
		
	/* 
	 * TEST to CHECK STATUS CHANGES
	 */
	
	// STATUS is changed
	if (flags != pflags) {
#ifdef DEBUG
		std::cerr << "sonosblobs UPDATE STATUS changed:" << std::endl;
		std::cerr << "\tPreviuos status was: "; printf("%d", pflags); std::cerr << std::endl;
		std::cerr << "\tCurrent status is: "; printf("%d", flags); std::cerr << std::endl;
		std::cerr << "\tFLAG STATUS" << std::endl;
		
		// check single flags
		if ((flags & SONOSEMPTY) == SONOSEMPTY) std::cerr << "\t\tSONOS EMPTY" << std::endl;
		if ((flags & BLOBSEMPTY) == BLOBSEMPTY) std::cerr << "\t\tBLOBS EMPTY" << std::endl;
		if ((flags & MORESONOS) == MORESONOS) std::cerr << "\t\tMORE SONOS" << std::endl;
		if ((flags & MOREBLOBS) == MOREBLOBS) std::cerr << "\t\tMORE BLOBS" << std::endl;
		if ((flags & EQUALSIZE) == EQUALSIZE) std::cerr << "\t\tEQUALSIZE" << std::endl;
#endif
	}
	
	// save pStatus (previuos frame memory)
	pflags = flags;
	
	// a questo livello ho i miei sonosBlobs per ulteriori loops
    
}

//--------------------------------------------------------------
// PARTICLES
//--------------------------------------------------------------
void sonosApp::makeParticle(int data, int position)
{
    if (physics.numberOfParticles() < MAXPARTICLES) {
        // la map dei sonos blob ha key a randomString, non so perche' FIXME
        // per ora copiamo in un vector

        std::vector <sonosBlob> blobs;
        
        for(map<string, sonosBlob>::iterator it = sonosblobs.begin(); it != sonosblobs.end(); ++it)
        {
            blobs.push_back(it->second);
        }
        
        std::sort(blobs.begin(),blobs.end(), sortByCentroid);
        
        if (position < blobs.size()) {
            // physics.makeParticle returns a particle pointer so you can customize it
            Physics::Particle2D *p = new Physics::Particle2D(Vec2f(blobs[position].avatar.x, blobs[position].avatar.y));
            
            // and set a bunch of properties (you don't have to set all of them, there are defaults)
            p->setMass(PARTICLEMASS)->setBounce(PARTICLEBOUNCE)->setRadius(PARTICLERADIUS)->enableCollision()->makeFree();
            
            int *xp = (int*) malloc(sizeof(int));
            *xp = data;
            p->data = xp;
            physics.addParticle(p);
        }
        //std::cerr << "DATA INPUT: " << *reinterpret_cast<int *>(p->data) << std::endl;
        
        // FIXME sto malloc da qualche parte sevre un dealloc
        

    }
}

void sonosApp::drawParticles()
{
    // test Physics
	ofPushStyle();
	ofEnableAlphaBlending();         
	for(int i=0; i<physics.numberOfParticles(); i++) {
		Physics::Particle2D *p = physics.getParticle(i);     
        // forse e' ok
        int * pt = reinterpret_cast<int *>(p->data);           
        switch (*pt) {
            //std::cerr << "DATA OUTPUT: " << *pt << std::endl;b    
            case 1:
                lettering1.setAnchorPoint(15,15);
                lettering1.draw(p->getPosition().x, p->getPosition().y);
                break;
            case 2:
                lettering2.setAnchorPoint(15,15);
                lettering2.draw(p->getPosition().x, p->getPosition().y);
                break;
            case 3:
                lettering3.setAnchorPoint(15,15);
                lettering3.draw(p->getPosition().x, p->getPosition().y);
                break;
            case 4:
                lettering4.setAnchorPoint(15,15);
                lettering4.draw(p->getPosition().x, p->getPosition().y);
                break;                
            default:
                break;
        }
	}
    ofDisableAlphaBlending();    
    ofPopStyle();
}

//--------------------------------------------------------------
//  OSC UPDATE/DRAW
//--------------------------------------------------------------

//--------------------------------------------------------------
void sonosApp::OscUpdate()
{
	
	// hide old messages
	for ( int i=0; i<NUM_MSG_STRINGS; i++ )
	{
		if ( timers[i] < ofGetElapsedTimef() )
			msg_strings[i] = "";
	}
	
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		// check for mouse moved message
		// midi type note channel 1
		if ( m.getAddress() == "/midi/note/1" )
		{
            if ( m.getArgAsInt32(2) == 1 ) {
                int nota = m.getArgAsInt32(0);
                switch (nota) {
                    case 57:
                        makeParticle(1,0);
                        break;
                    case 59:
                        makeParticle(2,0);		
                        break;
                    case 54:
                        makeParticle(3,0);				
                        break;
                    case 60:
                        makeParticle(4,0);				                    
                        break;
                    case 38:
                        makeParticle(1,1);			                    
                        break;
                    case 43:
                        makeParticle(2,1);
                        break;
                    case 42:
                        makeParticle(3,1);
                        break;
                    case 41:
                        makeParticle(4,1);
                        break;
                    default:
                        std::cerr << "Nota Sconosciuta: " << nota << std::endl;
                        break;
                }
            }
            
#ifdef DEBUG
            // DEBUG
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for ( int i=0; i<m.getNumArgs(); i++ )
			{
				// get the argument type
				msg_string += m.getArgTypeName( i );
				msg_string += ":";
				// display the argument - make sure we get the right type
				if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
					msg_string += ofToString( m.getArgAsInt32( i ) );
				else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
					msg_string += ofToString( m.getArgAsFloat( i ) );
				else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
					msg_string += m.getArgAsString( i );
				else
					msg_string += "unknown";
			}
            std::cerr << msg_string << std::endl;
#endif
            
		}
		else
		{
			// unrecognized message
		}
		
	}
	
	
}

//--------------------------------------------------------------


//--------------------------------------------------------------
void sonosApp::sonosBlobsInsert()
{
	// inserisco sonosBlobs
	for(int i = 0; i < contourFinder.blobs.size(); i++) {
		sonosBlob myblob = contourFinder.blobs[i];
		myblob.avatar.setStyle(randOfColor());
		sonosblobs.insert(std::pair<string, sonosBlob>(myblob.code,myblob));
	}	
}

//--------------------------------------------------------------
void sonosApp::sonosBlobsUpdate()
{	
	// Itero sui sonoblobs
	for(map<string, sonosBlob>::iterator i = sonosblobs.begin(); i != sonosblobs.end(); ++i)
	{
		
		// se non ci sono blobs in contourFinder picche
		if (contourFinder.blobs.size() == 0) return;
		
		// set initial MAX distance
		float distance = camWidth;
		int blobposition;
		
		// Itero sui CVblobs
		for ( int m=0; m < contourFinder.blobs.size(); m++ ) 
		{
			unsigned mdist = abs(contourFinder.blobs[m].centroid.x - i->second.centroid.x);
			// cerca minimal distance
			if (mdist < distance) {
				distance = mdist;
				blobposition = m;
			}
		}
		// update del sonosBlob con il CVblob piu' vicino (byCentroid)
		sonosblobs[i->first].update(contourFinder.blobs[blobposition]);
		//erase this CVblob
		contourFinder.blobs.erase(contourFinder.blobs.begin() + blobposition);
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
	for(map<string, sonosBlob>::iterator i = sonosblobs.begin(); i != sonosblobs.end(); ++i)
	{
		sonosBlob curr_blob = i->second;
		curr_blob.setBlobColor(BlobColor);
		curr_blob.drawPixels();
		
		// IF interface: draw rectangle around blob
		if(bInterface) curr_blob.drawRect();
		
		// set color by position (TEST)
		ofPushStyle();
		ofSetColor(curr_blob.avatar.color);		
		if (bCircle) curr_blob.circle();
		if (bLine) curr_blob.rectangle();
		ofPopStyle();
		
		if (bAvatar) {
			curr_blob.avatar.draw();
		}
		
	}
    
	if (bDrawImage==true){
        DrawImageCounter++;
        bDrawImage=false;
	}
	
	drawParticles();

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
void sonosApp::setFullScreen()
{
	if (bToogleFullScreen && !bFullscreen) {
		ofSetFullscreen(true);
		bFullscreen = true;
		bToogleFullScreen = false;
	} else if (bToogleFullScreen && bFullscreen) {
		bFullscreen = false;
		bToogleFullScreen = false;
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
