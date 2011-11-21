#include "laserinneApp.h"

class TSPSPersonAttributes {
public:
	TSPSPersonAttributes(){
		height = 0;
	}

	float height;
};

//--------------------------------------------------------------
void laserinneApp::setup(){
	ofDisableSmoothing();
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	ofBackground(223, 212, 190);
	
	camWidth = 640;
	camHeight = 480;
	
#ifdef _USE_LIVE_VIDEO
	pointGrey.setup();
	pointGrey.set1394b(true);
	pointGrey.setSize(1024,768);
	
	camWidth = pointGrey.getWidth();
	camHeight = pointGrey.getHeight();
#else
	vidPlayer.loadMovie("testmovie/800x600.mov");
	vidPlayer.play();
	
	camWidth = vidPlayer.width;
	camHeight = vidPlayer.height;
#endif

	colorImg.allocate(camWidth, camHeight);
	grayImg.allocate(camWidth, camHeight);
	
	std::cout << "Camera width: " << curFrame.getWidth() << " | Camera height: " << curFrame.getHeight() << std::endl;
	
	peopleTracker.setup(camWidth, camHeight);
	peopleTracker.loadFont("fonts/times.ttf", 10);
	peopleTracker.setListener( this );
	peopleTracker.setActiveDimensions( ofGetWidth(), ofGetHeight() - 68 );
	timesBoldItalic.loadFont("fonts/timesbi.ttf", 17);	
}

//--------------------------------------------------------------
void laserinneApp::update(){

	bool bNewFrame = false;
	
	#ifdef _USE_LIVE_VIDEO
	bNewFrame = pointGrey.grabVideo(curFrame);
	curFrame.update();
	#else
	vidPlayer.idleMovie();
	bNewFrame = vidPlayer.isFrameNew();
	#endif
	
	if(bNewFrame) {
		#ifdef _USE_LIVE_VIDEO
		grayImg.setFromPixels(curFrame.getPixels(), camWidth, camHeight);
		peopleTracker.update(grayImg);
		#else
		colorImg.setFromPixels(vidPlayer.getPixels(), camWidth, camHeight);
		peopleTracker.update(colorImg);
		#endif

		/*
		//iterate through the people
		for(int i = 0; i < peopleTracker.totalPeople(); i++){
			ofxTSPSPerson* p = peopleTracker.personAtIndex(i);
		}
		 */
		
	}
}

//delegate methods for people entering and exiting
void laserinneApp::personEntered( ofxTSPSPerson* newPerson, ofxTSPSScene* scene )
{
	newPerson->customAttributes = new TSPSPersonAttributes();

	//do something with them
	ofLog(OF_LOG_VERBOSE, "person %d of size %f entered!\n", newPerson->pid, newPerson->area);
	drawStatus[0] = 10;
}

void laserinneApp::personMoved( ofxTSPSPerson* activePerson, ofxTSPSScene* scene )
{

	//do something with the moving person
	ofLog(OF_LOG_VERBOSE, "person %d of moved to (%f,%f)!\n", activePerson->pid, activePerson->boundingRect.x, activePerson->boundingRect.y);
	drawStatus[1] = 10;
}

void laserinneApp::personWillLeave( ofxTSPSPerson* leavingPerson, ofxTSPSScene* scene )
{
	//do something to clean up
	ofLog(OF_LOG_VERBOSE, "person %d left after being %d frames in the system\n", leavingPerson->pid, leavingPerson->age);
	drawStatus[2] = 10;
}

void laserinneApp::personUpdated( ofxTSPSPerson* updatedPerson, ofxTSPSScene* scene )
{
	TSPSPersonAttributes* attrbs = (TSPSPersonAttributes*)updatedPerson->customAttributes;

	ofLog(OF_LOG_VERBOSE, "updated %d person\n", updatedPerson->pid);
	drawStatus[1] = 10;
}

//--------------------------------------------------------------
void laserinneApp::draw(){
	ofEnableAlphaBlending();
	ofSetHexColor(0xffffff);
	
	ofPushStyle();
	
	peopleTracker.draw();

	ofPopStyle();
}


//--------------------------------------------------------------
void laserinneApp::keyPressed  (int key){

	switch (key){
		case OF_KEY_ESC: {
			exit();
		}break;
		case ' ':{
			peopleTracker.relearnBackground();
		}break;
		case 'f':{
			ofToggleFullscreen();
		}break;
			
	}
}

//--------------------------------------------------------------
void laserinneApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void laserinneApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void laserinneApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void laserinneApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void laserinneApp::windowResized(int w, int h){
}

