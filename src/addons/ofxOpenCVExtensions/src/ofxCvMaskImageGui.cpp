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
	penDiameter = 30.0;
	
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

void ofxCvMaskImageGui::setPenDiameter(float diameter) {
	penDiameter = diameter;
}
 
void ofxCvMaskImageGui::maskImage(ofxCvGrayscaleImage* maskMe) {

	if( maskMe->getWidth()  == cvMaskImage.getWidth() && 
	    maskMe->getHeight() == cvMaskImage.getHeight() ) {
		cvCopy( cvTempImage.getCvImage(), maskMe->getCvImage(), cvMaskImage.getCvImage() );	
	 }
}

//----------------------------------------------------
void ofxCvMaskImageGui::readFromFile(string filePath){
	
	imageFile = filePath + ".png";
	if ( maskFile.loadImage(imageFile) ) maskFile.setImageType(OF_IMAGE_GRAYSCALE);
	cvMaskImage.setFromPixels(maskFile.getPixels(), maskFile.getWidth(), maskFile.getHeight());
	loadSettings();	
}

//----------------------------------------------------
void ofxCvMaskImageGui::loadSettings(){
	
}


//----------------------------------------------------
void ofxCvMaskImageGui::saveToFile(string filePath){
	string str;
	//std::cout << filePath << std::endl;
	maskFile.setFromPixels(cvMaskImage.getPixels(), cvMaskImage.getWidth(), cvMaskImage.getHeight(), OF_IMAGE_GRAYSCALE);
	imageFile = filePath + ".png";
	std::cout << "SAVING MASK " << imageFile << std::endl;
	maskFile.saveImage(imageFile);
}		

//----------------------------------------------------
void ofxCvMaskImageGui::draw(float passedX, float passedY, float scaleWidth, float scaleHeight){
}

//----------------------------------------------------
void ofxCvMaskImageGui::draw(){
}

//----------------------------------------------------
void ofxCvMaskImageGui::_mousePressed(ofMouseEventArgs &e){	
	if (bAdjustedView) {
		updateMask(e.x, e.y, x, y, width*scale.x, height*scale.y, (e.button != 0));
	}
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
}
//----------------------------------------------------
void ofxCvMaskImageGui::onReleaseOutside(int mouseX, int mouseY, int button){
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
		cvCircle( cvMaskImage.getCvImage(), cvPoint(px, py), (int)penDiameter, cvScalarAll(val), -1 );
		return true;
	}
	
	return false;
};