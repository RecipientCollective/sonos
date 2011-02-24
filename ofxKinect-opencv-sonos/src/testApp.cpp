#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
	
videosurce = false;	
	
	if (videosurce) {
		camWidth = 640;
		camHeight = 480;
		vidGrabber.setVerbose(true);
		vidGrabber.initGrabber(camWidth, camHeight, false);
		
		colorImg.allocate(camWidth, camHeight);
		grayImage.allocate(camWidth, camHeight);
		grayBg.allocate(camWidth,camHeight);
		grayDiff.allocate(camWidth,camHeight);
		
		bLearnBakground = true;
		Threshold = 50;
				
		
		
		bThreshWithOpenCV = false;

		
	} else {
		//shows infrared image
		kinect.init(true);
		kinect.setVerbose(true);
		kinect.open();
		
		colorImg.allocate(kinect.width, kinect.height);
		grayImage.allocate(kinect.width, kinect.height);
		grayThresh.allocate(kinect.width, kinect.height);
		grayThreshFar.allocate(kinect.width, kinect.height);
		
		nearThreshold = 50;
		farThreshold  = 200;
		bThreshWithOpenCV = true;
		
		angle = 0;
		kinect.setCameraTiltAngle(angle);
	
	}

	
		
	ofSetFrameRate(60);

	
	colorz=1;
	contour_min = 100;
	scale_x = 1.0;
	scale_y = 1.0;
	mtrx = 1.0;
	mtry = 1.0;
	interface = true;
	circle = true;
	cloud = false;
	debug = false;
	
	rectangle = false;
}

//--------------------------------------------------------------
void testApp::update()
{
	ofBackground(100, 100, 100);
	if (videosurce) {
		bool bNewFrame = false;
		
		vidGrabber.grabFrame();
		bNewFrame = vidGrabber.isFrameNew();
		
		if (bNewFrame){
		colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
			grayImage = colorImg;
			if (bLearnBakground == true){
				grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
				bLearnBakground = false;
			}
			
			// take the abs value of the difference between background and incoming and then threshold:
			grayDiff.absDiff(grayBg, grayImage);
			grayDiff.threshold(Threshold);
			}
		
		
		
	} else {
		kinect.update();
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		//we do two thresholds - one for the far plane and one for the near plane
		//we then do a cvAnd to get the pixels which are a union of the two thresholds.	
		if( bThreshWithOpenCV ){
			grayThreshFar = grayImage;
			grayThresh = grayImage;
			grayThreshFar.threshold(farThreshold, true);
			grayThresh.threshold(nearThreshold);
			cvAnd(grayThresh.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		}else{
			
			//or we do it ourselves - show people how they can work with the pixels
			
			unsigned char * pix = grayImage.getPixels();
			int numPixels = grayImage.getWidth() * grayImage.getHeight();
			
			for(int i = 0; i < numPixels; i++){
				if( pix[i] > nearThreshold && pix[i] < farThreshold ){
					pix[i] = 255;
				}else{
					pix[i] = 0;
				}
			}
		}
		
	}
		//update the cv image
	grayImage.flagImageChanged();

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
	if (videosurce){
	contourFinder.findContours(grayDiff, 20, (camWidth*camHeight)/3, 3, false);
	} else {
    contourFinder.findContours(grayImage, contour_min, (kinect.width*kinect.height)/3, 3, false, true);
	}
}

//--------------------------------------------------------------
void testApp::draw()
{		

	ofPushMatrix();
	ofScale(scale_x, scale_y, 1.0);
	ofTranslate(mtrx, mtry, 1.0);

	if (debug) {
		 
		if (videosurce){
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
			kinect.drawDepth(10, 10, 400, 300);
			kinect.draw(420, 10, 400, 300);
			contourFinder.draw(10, 320, 400, 300);	
		}


		
	} else {
		
		ofSetColor(205, 205, 205);
		
		if (videosurce){
			grayDiff.draw(0,0, camWidth, camHeight);

			//colorImg.draw(20,20, camWidth, camHeight);
			//grayImage.draw(0, 0, camWidth, camHeight);
		} else {
		
		grayImage.draw(0, 0, kinect.width, kinect.height);
	}
		
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
				float raggio = (blobheight >= blobwidth ? blobheight : blobwidth) / 2.0;
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

	

	// point cloud is commented out because we need a proper camera class to explore it effectively	
	if (cloud) {
		drawPointCloud();
	}
	
	ofPopMatrix();
	
	ofSetColor(255, 255, 255);
	
	// INTERFACE DEBUG
	if (interface) {
		ofDrawBitmapString("INTERFACE (press: h to hide)", 20, 20);
		ofDrawBitmapString("accel is: " + ofToString(kinect.getMksAccel().x, 2) + " / " 
						   + ofToString(kinect.getMksAccel().y, 2) + " / "
						   + ofToString(kinect.getMksAccel().z, 2), 20, 58 );
		
		char reportStr[1024];
		sprintf(reportStr, "using opencv threshold = %i (press spacebar)\nset near threshold %i (press: + -)\nset far threshold %i (press: < >) num blobs found %i, fps: %f",bThreshWithOpenCV, nearThreshold, farThreshold, contourFinder.nBlobs, ofGetFrameRate());
		ofDrawBitmapString(reportStr, 20, 90);
		ofDrawBitmapString("tilt angle: " + ofToString(angle) + " (press: s x). Contour (poress: t y): " + ofToString(contour_min) ,20,130);
		ofDrawBitmapString("scale: " + ofToString(scale_x) + "," + ofToString(scale_y) + " (press: a z)", 20,160);
		ofDrawBitmapString("translate: " + ofToString(mtrx) + "," + ofToString(mtry) + " (press: ARROWS)", 20,190);			
		ofDrawBitmapString("d: debug mode. c: toggle circle. r: reset scale and translate. p: point cloud", 20,220);

	}
}

void testApp::drawPointCloud() {
	ofScale(400, 400, 400);
	int w = 640;
	int h = 480;
	ofRotateY(mouseX);
	float* distancePixels = kinect.getDistancePixels();
	glBegin(GL_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			ofPoint cur = kinect.getWorldCoordinateFor(x, y);
			glVertex3f(cur.x, cur.y, cur.z);
		}
	}
	glEnd();
}

//--------------------------------------------------------------
void testApp::exit(){
	kinect.close();
}

//--------------------------------------------------------------
void testApp::keyPressed (int key)
{
	switch (key)
	{
		case ' ':
			bLearnBakground = true;
			bThreshWithOpenCV = !bThreshWithOpenCV;
		break;
	
		case '>':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
		case '<':				
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			Threshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
		case '-':		
			nearThreshold --;
			Threshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;

		case 's':
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;

		case 'x':
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
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
			cloud = !cloud;
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
			videosurce = !videosurce;
			setup();
			break;
		case ',':
			if (videosurce){
				vidGrabber.videoSettings();
			
			}
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

