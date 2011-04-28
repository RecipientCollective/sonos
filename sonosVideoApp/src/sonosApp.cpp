#include "sonosApp.h"

//--------------------------------------------------------------
void sonosApp::setup(){
	
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
	bThreshWithOpenCV = true;      //treshold with opencv or not
	ofSetCircleResolution(40); 	   //set resolution of circle
	ofEnableSmoothing();	
	ofSetFrameRate(60);
	
	//for smooth animation, set vertical sync if we can
	//ofSetVerticalSync(true);
	
	//colors setup
	BckColor=1;
	BlobColor=0xDD00CC;
	colorz=1;
	
	blobMax=2;
	contour_min = 350;
	scale_x = 1.0;
	scale_y = 1.0;
	mtrx = 1.0;
	mtry = 1.0;
	interface = true;
	circle = true;
	debug = true;
	rectangle = false;
	
}

//--------------------------------------------------------------
void sonosApp::update(){
	
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
void sonosApp::draw(){
	if (debug) {
		debugDraw();
	} else {
		// background
		sonosApp::background(BckColor);
	}
}

//--------------------------------------------------------------
void sonosApp::exit(){
	//magari c' da chiudere la cam o i video da verificare;
	OF_EXIT_APP(0);
}

//--------------------------------------------------------------
void sonosApp::debugDraw(){
	ofPushMatrix();
	ofScale(0.5, 0.5, 1.0);
	ofBackground(0, 0, 0);
	colorImg.draw(20,20);
	grayImage.draw((camWidth+20),20);
	grayBg.draw(20,(camHeight+20));
	grayDiff.draw((camWidth+20),(camHeight+20));
	ofPopMatrix();
}

//--------------------------------------------------------------
void sonosApp::background(int color){
	
	switch (color)
	{
		case 1:
			ofBackground(100, 100, 100);
			break;
			
		case 2:
			ofBackground(0, 0, 0);
			break;
	}	
	
}

//--------------------------------------------------------------
void sonosApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void sonosApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void sonosApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void sonosApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void sonosApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void sonosApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void sonosApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void sonosApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void sonosApp::dragEvent(ofDragInfo dragInfo){ 
	
}