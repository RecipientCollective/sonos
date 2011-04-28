#include "sonosApp.h"

//--------------------------------------------------------------
void sonosApp::setup(){
	filename = "videos/retro-ir.mov";
	
#ifdef _USE_LIVE_VIDEO
	camWidth = 640;
	camHeight = 480;
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth, camHeight, false);
#else
	//check if file exists
	bool bFileThere = false;
	fstream fin;
	string fileNameInOF = ofToDataPath(filename); // since OF files are in the data directory, we need to do this
	fin.open(fileNameInOF.c_str(),ios::in);
	if ( fin.is_open() ) {
		cout<<"file exists"<<endl;
		bFileThere =true;
	}
	fin.close();
	
	if (bFileThere) {
		vidPlayer.loadMovie(filename);
		vidPlayer.play();
		camWidth = vidPlayer.getWidth();
		camHeight = vidPlayer.getHeight();
	} else {
		cout << "File" << fileNameInOF << " is not here!" << endl;
		sonosApp:exit();
	}
	
#endif
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