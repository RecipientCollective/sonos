#include "testApp.h"

// grezzo test per grezzo man
bool sortByCentroid(const ofxCvBlob& d1, const ofxCvBlob& d2)
{
	return d1.centroid.x < d2.centroid.x;
}


//--------------------------------------------------------------
void testApp::setup()
{
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
	testApp:exit();
	}
		
#endif
	
	colorImg.allocate(camWidth, camHeight);
	grayImage.allocate(camWidth, camHeight);
	grayBg.allocate(camWidth,camHeight);
	grayDiff.allocate(camWidth,camHeight);
	
	bLearnBakground = true;
	Threshold = 50;
	//treshold with opencv or not
	bThreshWithOpenCV = true;
	//set resolution of circle
	ofSetCircleResolution(40);
	//for smooth animation, set vertical sync if we can
	//ofSetVerticalSync(true);
	ofEnableSmoothing();
	
	ofSetFrameRate(60);
	
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
void testApp::update()
{
	
	bool bNewFrame = false;
	
#ifdef _USE_LIVE_VIDEO
	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();
#else
	vidPlayer.idleMovie();
	bNewFrame = vidPlayer.isFrameNew();
#endif
	
	
	
	if (bNewFrame){
		
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
		
		//update the cv image
		grayImage.flagImageChanged();
		grayDiff.flagImageChanged();
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		/*
		 ofxCvContourFinder::findContours( ofxCvGrayscaleImage&  input,
		 int minArea,
		 int maxArea,
		 int nConsidered,
		 bool bFindHoles,
		 bool bUseApproximation
		 )
		 */
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, contour_min, (camWidth*camHeight)/3, blobMax, false, true);
		
		//float max_x = 0;
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
void testApp::draw()
{		
	// background
	testApp::background(BckColor);
	
	ofPushMatrix();
	ofScale(scale_x, scale_y, 1.0);
	ofTranslate(mtrx, mtry, 1.0);
	
	if (debug) {
		
		
		colorImg.draw(20,20);
		grayImage.draw(360,20);
		grayBg.draw(20,280);
		grayDiff.draw(360,280);
		
		// then draw the contours:
		
		ofFill();
		ofSetColor(0x333333);
		ofRect(360,540,320,240);
		ofSetColor(0xffffff);
		
		// we could draw the whole contour finder
		//contourFinder.draw(360,540);
		
		// or, instead we can draw each blob individually,
		// this is how to get access to them:
		for (int i = 0; i < contourFinder.nBlobs; i++){
			contourFinder.blobs[i].draw(360,540);
		}
		
	} else {
		
		
		// in draw we iterate in the map
		for(map<int, sonosBlob>::iterator i = sonosblobs.begin(); i != sonosblobs.end(); ++i)
		{
			int index = i->first;
			sonosBlob curr_blob = i->second;
			
			float cx = curr_blob.centroid.x;
			float cy = curr_blob.centroid.y;
			float blobarea = curr_blob.area;
			float blobheight = curr_blob.boundingRect.height;	
			float blobwidth = curr_blob.boundingRect.width;	
			float rectx = curr_blob.boundingRect.x;
			float recty = curr_blob.boundingRect.y;
			
			
			//drawing only pixels form blobs, extracted from conturfinder
			ofPushStyle();
			ofSetColor(BlobColor);
			glPushMatrix();
			ofBeginShape();
			for( int j=0; j<curr_blob.nPts; j++ ) {
				ofVertex( curr_blob.pts[j].x, curr_blob.pts[j].y );
			}
			ofEndShape();
				
			glPopMatrix();
			ofPopStyle();
			
			if (i->first == 0) {
				ofSetColor(255, 0, 0);
			} else if (i->first == 1) {
				ofSetColor(0, 255, 0);
			} else if (i->first == 2) {
				ofSetColor(0, 0, 255);
			} else {
				ofSetColor(100, 100, 150);
			}
			
			
			if(interface){
				ofNoFill();
				ofRect(rectx,recty,blobwidth,blobheight);
				ofFill();
			}
			
			if (circle) {
				float raggio = (blobheight >= blobwidth ? blobheight : blobwidth) / 1.5;
				ofCircle( cx, cy, raggio);
			}

			if (rectangle) {
				ofRect(rectx,0,blobwidth, 480);
			}
			
		}
		
	}
	
	
	ofPopMatrix();
	
	
	
	// INTERFACE DEBUG
	if (interface) {
		
		
		ofSetColor(255, 255, 255);
		ofDrawBitmapString("INTERFACE (press: h to hide)", 20, 20);
		char reportStr[1024];
		sprintf(reportStr, "using opencv threshold = %i (press: s)\nset threshold %i (press: + -)\nnum blobs found %i,color: %i , fps: %f",bThreshWithOpenCV, Threshold, contourFinder.nBlobs, colorz, ofGetFrameRate());
		ofDrawBitmapString(reportStr, 20, 90);
		ofDrawBitmapString("color number: " + ofToString(colorz)+ " (press: 1,2,3,4). Contour (press: t y): " + ofToString(contour_min) +" MaxBlobs (press: < >): "+ofToString(blobMax) ,20,130);
		ofDrawBitmapString("scale: " + ofToString(scale_x) + "," + ofToString(scale_y) + " (press: a z)", 20,160);
		ofDrawBitmapString("translate: " + ofToString(mtrx) + "," + ofToString(mtry) + " (press: ARROWS)", 20,190);			
		ofDrawBitmapString("d: debug mode. c: toggle circle. r: reset scale and translate. ,: video settings", 20,220);
		
	}
}


//--------------------------------------------------------------
void testApp::exit(){
	//magari c'� da chiudere la cam o i video da verificare;
	std::exit(0);
}


void testApp::background(int color){

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
void testApp::keyPressed (int key)
{
	switch (key)
	{
		case '1':
			BckColor=1;
			break;
			
		case '2':
			BckColor=2;
			break;
			
		case ' ':
			bLearnBakground = true;
			break;
			
		case '>':
			blobMax ++;
			break;
		case '<':	
			blobMax --;
			break;
			
		case '+':
			Threshold ++;
			if (Threshold > 255) Threshold = 255;
			break;
		case '-':
			Threshold --;
			if (Threshold < 0) Threshold = 0;
			break;
		case 'w':
			
			break;
			
		case 's':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;
			
		case 'x':
			
			break;
			
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
			
		case 'f':
			bFullscreen = !bFullscreen;
			if (!bFullscreen){
				ofSetWindowShape(1024,768);
				ofSetFullscreen(false);
				// figure out how to put the window in the center:
				int screenW = ofGetScreenWidth();
				int screenH = ofGetScreenHeight();
				ofSetWindowPosition(screenW/2-1024/2, screenH/2-768/2);
			}
			else if (bFullscreen == 1){
				ofSetFullscreen(true);
			}
			break;
			
		case 'a':
			scale_x+=0.01;
			scale_y+=0.01;
			break;
		case 'z':
			scale_x-=0.01;
			scale_y-=0.01;
			break;
		case 'h':
			interface = !interface;
			break;
		case 'c':
			circle = !circle;
			break;
		case 'p':
			vidPlayer.close();
			currentVideo++;
			if (currentVideo==nVideos) {
				currentVideo=0;
			}
			setup();
			break;
		case 'o':
			vidPlayer.close();
			currentVideo--;
			if (currentVideo<0) {
				currentVideo=nVideos;
			}
			setup();
			break;
			
		case 'd':
			debug = !debug;
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
		case 'q':
			rectangle = !rectangle;
			break;	
		case 'v':
			break;
		case ',':
#ifdef _USE_LIVE_VIDEO
			vidGrabber.videoSettings();
#else
			//
#endif
			break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

