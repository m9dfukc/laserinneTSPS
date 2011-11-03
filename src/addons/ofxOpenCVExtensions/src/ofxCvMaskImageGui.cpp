/*
 *  ofxCvMaskImageGui.cpp
 *  laserinneTSPS
 *
 *  Created by Andreas on 31.08.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxCvMaskImageGui.h"

//----------------------------------------------------
ofxCvMaskImageGui::ofxCvMaskImageGui(){
	selected = -1;
	quadName = "QUAD2_";
	scale.x = scale.y = 1.0f;
	bAutoSave = false;
	ofAddListener(ofEvents.mousePressed, this, &ofxCvMaskImageGui::_mousePressed);
	ofAddListener(ofEvents.mouseReleased, this, &ofxCvMaskImageGui::_mouseReleased);
	ofAddListener(ofEvents.mouseDragged, this, &ofxCvMaskImageGui::_mouseDragged);
}

//----------------------------------------------------
void ofxCvMaskImageGui::setup(string _quadName, int _width, int _height){
	quadName = _quadName;
	bAdjustedView = false;	
	width = _width;
	height = _height;
	
	maskFile.allocate(_width, _height, OF_IMAGE_GRAYSCALE);
	cvTempImage.allocate( maskFile.getWidth(), maskFile.getHeight() );
	cvMaskImage.allocate( maskFile.getWidth(), maskFile.getHeight() );
	
	cvFillImage(cvMaskImage.getCvImage(), 255.0);
	cvZero(cvTempImage.getCvImage());
	
	cvMaskImage.flagImageChanged();
	cvTempImage.flagImageChanged();
	
	cvMaskImage.setFromPixels( maskFile.getPixelsRef() );
}

/*
ofxCvGrayscaleImage ofxCvMaskImageGui::getMask() {
	
}
*/

void ofxCvMaskImageGui::clearMask() {
	cvFillImage( cvMaskImage.getCvImage(), 0.0 );
}
 
void ofxCvMaskImageGui::maskImage(ofxCvGrayscaleImage* maskMe) {

	if( maskMe->getWidth()  == cvMaskImage.getWidth() && 
	    maskMe->getHeight() == cvMaskImage.getHeight() ) {
		cvCopy( cvTempImage.getCvImage(), maskMe->getCvImage(), cvMaskImage.getCvImage() );	
	 }
}

//----------------------------------------------------
void ofxCvMaskImageGui::readFromFile(string filePath){
	
	xmlFile = filePath + ".xml";
	imageFile = filePath + ".jpg";
	
	if ( maskFile.loadImage(imageFile) ) maskFile.setImageType(OF_IMAGE_GRAYSCALE);
	xml.loadFile(xmlFile);

	loadSettings();	
}

//----------------------------------------------------
void ofxCvMaskImageGui::loadSettings(){
	
}


//----------------------------------------------------
void ofxCvMaskImageGui::saveToFile(string filePath){
	string str;
	
	xmlFile = filePath + ".xml";
	imageFile = filePath + ".jpg";
	xml.saveFile(xmlFile);
	maskFile.saveImage(filePath);
}		

//----------------------------------------------------
void ofxCvMaskImageGui::draw(float passedX, float passedY, float scaleWidth, float scaleHeight){
	//draw(passedX, passedY, scaleWidth, scaleHeight, 255, 255, 255, 1);
}

//----------------------------------------------------
void ofxCvMaskImageGui::draw(){
	//draw(x, y, width*scale.x, height*scale.y, 255, 255, 0, 1);
}

//----------------------------------------------------
void ofxCvMaskImageGui::_mousePressed(ofMouseEventArgs &e){	
	if (bAdjustedView) {
		updateMask(e.x, e.y, x, y, width*scale.x, height*scale.y, (e.button != 0));
	}
	//selectPoint(e.x, e.y, x, y, width*scale.y, height*scale.y, 40);
}

//----------------------------------------------------
void ofxCvMaskImageGui::setAutoSave( bool autoSave){
	bAutoSave = autoSave;
}

//----------------------------------------------------
void ofxCvMaskImageGui::_mouseDragged(ofMouseEventArgs &e){
	if (bAdjustedView) {
		updateMask(e.x, e.y, x, y, width*scale.x, height*scale.y, (e.button != 0));
	}
}

//----------------------------------------------------
void ofxCvMaskImageGui::_mouseReleased(ofMouseEventArgs &e){
	/*
	if (bAutoSave && bAdjustedView){
		if (xmlFile != "") saveToFile(xmlFile);
		else saveToFile("guiquad-settings.xml");
	}
	 */
}
//----------------------------------------------------
void ofxCvMaskImageGui::onReleaseOutside(int mouseX, int mouseY, int button){
	/*
	if (bAutoSave){
		if (xmlFile != "") {
			saveToFile(xmlFile);
		} else {
			saveToFile("maskiamge-settings.xml");
		}
	}
	 */
}

//----------------------------------------------------
void ofxCvMaskImageGui::setScale( float scaleX, float scaleY ){
	scale.x = scaleX;
	scale.y = scaleY;
};

//----------------------------------------------------
bool ofxCvMaskImageGui::updateMask(float passedX, float passedY, float offsetX, float offsetY, float scaleWidth, float scaleHeight, bool erase){
	
	if(scaleWidth == 0 || scaleHeight == 0){
		return false;
	}  
	
	if( passedX < offsetX )					passedX = offsetX;
	if( passedX > offsetX + scaleWidth ) 	passedX = offsetX + scaleWidth;
	if( passedY < offsetY )					passedY = offsetY;
	if( passedY > offsetY + scaleHeight) 	passedY = offsetY + scaleHeight;
	
	//lets get it in range x(0 - scaleWidth) y(0 - scaleHeight)
	int px = (int)(((float)passedX - (float)offsetX) / scale.x);
	int py = (int)(((float)passedY - (float)offsetY) / scale.y);
	
	if (px > 0 && px < width && py > 0 && py < height) {  
		double val = erase ? 0.0 : 255.0;
		cvCircle( cvMaskImage.getCvImage(), cvPoint(px, py), 30, cvScalarAll(val), -1 );
		return true;
	}
	
	return false;
};