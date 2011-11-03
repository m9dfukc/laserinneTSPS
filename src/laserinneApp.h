#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "opencv2/cvconfig.h"
#include "ofxCv.h"
#include "ofxTSPS.h"
#include "ofxLibdc.h"

//#define _USE_LIVE_VIDEO		// uncomment this to use a normal OF camera

class  laserinneApp: public ofBaseApp, public ofxPersonListener {

	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void personEntered( ofxTSPSPerson* newPerson, ofxTSPSScene* scene );
		void personMoved( ofxTSPSPerson* activePerson, ofxTSPSScene* scene );
		void personWillLeave( ofxTSPSPerson* leavingPerson, ofxTSPSScene* scene );
		void personUpdated( ofxTSPSPerson* updatedPerson, ofxTSPSScene* scene );
		
		ofImage curFrame;
		int camWidth, camHeight;

		ofxCvColorImage		colorImg;
		ofxCvGrayscaleImage	grayImg;
	
#ifdef _USE_LIVE_VIDEO
		ofxLibdc::Camera camera;
		ofxLibdc::Grabber grabber;
		ofxLibdc::PointGrey pointGrey;	
#else
		ofVideoPlayer 		vidPlayer;
#endif
	
	
	//status bar stuff
		ofImage statusBar;
		int		drawStatus[3];
		ofImage personEnteredImage;
		ofImage personUpdatedImage;
		ofImage personLeftImage;
		ofTrueTypeFont timesBoldItalic;
	
	//other gui images
		ofImage background;


	ofxTSPSPeopleTracker peopleTracker;
	
	//example test values
	int itestValue;
	float ftestValue;
	bool btestValue;
	
};

#endif
