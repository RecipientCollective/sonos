#include "testApp.h"


#include <Carbon/Carbon.h>

//--------------------------------------------------------------
void testApp::setup()
{
	
	
	
#ifdef _USE_LIVE_VIDEO
	camWidth = 640;
	camHeight = 480;
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth, camHeight, false);
#else
	vidPlayer = new ofVideoPlayer();
	vidPlayer->loadMovie("retro-ir.mov");
	vidPlayer->play();
	camWidth = vidPlayer->getWidth();
	camHeight = vidPlayer->getHeight();
#endif
	
	
	openVideo = FALSE;
	
	
	colorImg.allocate(camWidth, camHeight);
	grayImage.allocate(camWidth, camHeight);
	grayBg.allocate(camWidth,camHeight);
	grayDiff.allocate(camWidth,camHeight);
	
	bLearnBakground = true;
	Threshold = 50;
	
	bThreshWithOpenCV = true;
	
	ofSetFrameRate(60);
	
	
	colorz=1;
	blobMax=2;
	contour_min = 100;
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
	ofBackground(100, 100, 100);
	bool bNewFrame = false;
	
	
	
	if (openVideo) {
		vidPlayer->stop();
		vidPlayer->close();
		delete vidPlayer;
		
		
		string URL;
		openFile(URL);
		cout << "URL: " << URL << endl; 
		
		vidPlayer = new ofVideoPlayer();
		vidPlayer->loadMovie(URL);
		vidPlayer->play();
		camWidth = vidPlayer->getWidth();
		camHeight = vidPlayer->getHeight();
		openVideo = FALSE;
		
	
	}
	
#ifdef _USE_LIVE_VIDEO
	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();
#else
	vidPlayer->idleMovie();
	bNewFrame = vidPlayer->isFrameNew();
#endif
	
	
	
	if (bNewFrame){
		
#ifdef _USE_LIVE_VIDEO
		colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
#else
		colorImg.setFromPixels(vidPlayer->getPixels(), camWidth,camHeight);
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
		//grayImage.flagImageChanged();
		
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
	
	}
}

//--------------------------------------------------------------
void testApp::draw()
{		
	
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
		ofSetColor(205, 205, 205);
		grayDiff.draw(0,0, camWidth, camHeight);
		//colorImg.draw(20,20, camWidth, camHeight);
		//grayImage.draw(0, 0, camWidth, camHeight);
		
		for(int i = 0; i < contourFinder.blobs.size(); i++) {
			
			switch(colorz){
				case 1:
					ofSetColor(255, 255, 100);
					break;
				case 2:
					ofSetColor(255, 100, 255);
					break;
				case 3:
					ofSetColor(100, 255, 255);
					break;
				case 4:
					ofSetColor(100, 120, 150);
					break;
			}
			
			ofxCvBlob curr_blob = contourFinder.blobs[i];
			float cx = curr_blob.centroid.x;
			float cy = curr_blob.centroid.y;
			float blobarea = curr_blob.area;
			float blobheight = curr_blob.boundingRect.height;	
			float blobwidth = curr_blob.boundingRect.width;	
			float rectx = curr_blob.boundingRect.x;
			float recty = curr_blob.boundingRect.y;
			
			ofNoFill();
			ofRect(rectx,recty,blobwidth,blobheight);
			ofFill();
			
			//ofSetColor(255, 255, 255);
			if (circle) {
				float raggio = (blobheight >= blobwidth ? blobheight : blobwidth) / 1.5;
				ofCircle( cx, cy, raggio);
			}
			if (rectangle) {
				ofRect(rectx,0,blobwidth, 480);
			}
			
			
			//if (interface) {
			//ofSetColor(255, 255, 255);
			//ofDrawBitmapString("blob "+ ofToString(i, 0) + ": try " + ofToString(cy, 2) + " /trx " + ofToString(cx, 2) + " /area " + ofToString(blobarea, 0) , 20, 200+i*10 );
			//}
		}
		
	}
	
	
	ofPopMatrix();
	
	ofSetColor(255, 255, 255);
	
	// INTERFACE DEBUG
	if (interface) {
		ofDrawBitmapString("INTERFACE (press: h to hide)", 20, 20);
		char reportStr[1024];
		sprintf(reportStr, "using opencv threshold = %i (press: s)\nset threshold %i (press: + -)\nnum blobs found %i,color: %i , fps: %f",bThreshWithOpenCV, Threshold, contourFinder.nBlobs, colorz, ofGetFrameRate());
		ofDrawBitmapString(reportStr, 20, 90);
		ofDrawBitmapString("color number: " + ofToString(colorz)+ " (press: 1,2,3,4). Contour (press: t y): " + ofToString(contour_min) +" MaxBlobs (press: < >): "+ofToString(blobMax) ,20,130);
		ofDrawBitmapString("scale: " + ofToString(scale_x) + "," + ofToString(scale_y) + " (press: a z)", 20,160);
		ofDrawBitmapString("translate: " + ofToString(mtrx) + "," + ofToString(mtry) + " (press: ARROWS)", 20,190);			
		ofDrawBitmapString("d: debug mode. c: toggle circle. r: reset scale and translate. ,: video settings o: open file", 20,220);
		
	}
}


//--------------------------------------------------------------
void testApp::exit(){
	//magari c'è da chiudere la cam o i video da verificare;
}

//--------------------------------------------------------------


int testApp::openFile(string& URL)
{
	short fRefNumOut;
	FSRef output_file;
	OSStatus err;
	
	NavDialogCreationOptions options;
	NavGetDefaultDialogCreationOptions( &options );
	options.modality = kWindowModalityAppModal;
	// adding a banner
	// options.message =  CFStringCreateWithCString(kCFAllocatorDefault, "hello world", kCFStringEncodingMacRoman);
	NavDialogRef dialog;
	
	err = NavCreateGetFileDialog(&options, NULL, NULL ,NULL, NULL, NULL, &dialog);
	err = NavDialogRun(dialog);
	
	NavUserAction action;
	action = NavDialogGetUserAction( dialog );
	
	if (action == kNavUserActionNone || action == kNavUserActionCancel) {
		cout << "no action or action cancel" << endl;
		return 0;
	}
	
	// get dialog reply
	NavReplyRecord reply;
	err = NavDialogGetReply(dialog, &reply);
	if ( err != noErr ){
		cout << "error getting DialogReply" << endl;
		return 0;
	}
	if ( reply.replacing )
	{
		cout << (" need to replace\n ") << endl;
	}
	
	AEKeyword keyword;
	DescType actual_type;
	Size actual_size;
	FSRef output_dir;
	err = AEGetNthPtr(&(reply.selection), 1, typeFSRef, &keyword, &actual_type,
					  &output_dir, sizeof(output_file), &actual_size);
	
	CFURLRef cfUrl = CFURLCreateFromFSRef( kCFAllocatorDefault, &output_dir );
	CFStringRef cfString = NULL;
	if ( cfUrl != NULL )
	{ 
		cfString = CFURLCopyFileSystemPath( cfUrl, kCFURLPOSIXPathStyle );
		CFRelease( cfUrl );
	}
	
	// copy from a CFString into a local c string (http://www.carbondev.com/site/?page=CStrings+)
	const int kBufferSize = 255;
	
	char fileURL[kBufferSize];
	Boolean bool1 = CFStringGetCString(cfString,fileURL,kBufferSize,kCFStringEncodingMacRoman);
	
	URL = fileURL;
	cout << URL << endl;
	
	// cleanup dialog
	NavDialogDispose(dialog);
	// dispose of reply:
	NavDisposeReply(&reply);
	// dispose of cfString
	CFRelease( cfString );
return 1;

}


void testApp::keyPressed (int key)
{
	switch (key)
	{
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
			
		case '1':
			colorz=1;
			break;
			
		case '2':
			colorz=2;
			break;
			
		case '3':
			colorz=3;
			
			break;
		case '4':
			colorz=4;
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
			case 'o':
			openVideo = TRUE;
			
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

