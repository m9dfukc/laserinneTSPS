/*
 *  ofxCvMaskImageGui.h
 *  laserinneTSPS
 *
 *  Created by Andreas on 31.08.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"
#include "ofxMSAInteractiveObject.h"

class ofxCvMaskImageGui : public ofxMSAInteractiveObject {
	
public:
	ofxCvMaskImageGui();
	
	void setup(string _quadName, int _width, int _height);
	void readFromFile(string filePath);
	void loadSettings();
	
	//ofxCvGrayscaleImage getMask();
	
	void maskImage(ofxCvGrayscaleImage* maskMe);
	bool updateMask(float passedX, float passedY, float offsetX, float offsetY, float scaleWidth, float scaleHeight, bool erase);
	void clearMask();
	void setPenDiameter(float diameter);
	
	
	void saveToFile(string filePath);
	void draw(float x, float y, float width, float height);
	void draw();
	
	void setAutoSave( bool autoSave=true);
	
	//Added MSAInteractiveObject stuff to simplify
	void _mousePressed(ofMouseEventArgs &e);
	void _mouseDragged(ofMouseEventArgs &e);
	void _mouseReleased(ofMouseEventArgs &e);
	void onReleaseOutside(int x, int y, int button);
	
	//added scaling
	void setScale( float scaleX, float scaleY );
	ofPoint scale;
	
	// ZACK BOKA: Keep track of current OpenTSPS view so that image warping only occurs when in camera view.
	bool bAdjustedView;
	
protected:
	ofxXmlSettings xml;
	string xmlFile;
	string imageFile;
	
	ofImage maskFile;
	
	ofxCvGrayscaleImage cvMaskImage;
	ofxCvGrayscaleImage cvTempImage;
	
	string quadName;
	//int width, height;
	bool bAutoSave;
	int selected;
	float penDiameter;
};


