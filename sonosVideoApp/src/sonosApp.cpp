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
		bFileThere =true;
	}
	fin.close();
	
	if (bFileThere) {
		vidPlayer.loadMovie(filename);
		vidPlayer.play();
		camWidth = vidPlayer.getWidth();
		camHeight = vidPlayer.getHeight();
	} else {
		cerr << "File " << fileNameInOF << " is not here!" << endl;
		sonosApp:exit();
	}
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
	debug = false;
	rectangle = false;
	
}

//--------------------------------------------------------------
void sonosApp::update(){
}

//--------------------------------------------------------------
void sonosApp::draw(){
	
}

//--------------------------------------------------------------
void sonosApp::exit(){
	//magari c' da chiudere la cam o i video da verificare;
	OF_EXIT_APP(0);
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

//--------------------------------------------------------------
bool sonosApp::sortByCentroid(const ofxCvBlob& d1, const ofxCvBlob& d2)
{
	return d1.centroid.x < d2.centroid.x;
}