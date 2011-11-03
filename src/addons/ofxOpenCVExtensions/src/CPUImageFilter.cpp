/*
*  CPUImageFilter.h
*  
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#include "CPUImageFilter.h"
#include "ofxCvColorImage.h"
#include "ofxCvGrayscaleImage.h"
#include "ofxCvFloatImage.h"

//--------------------------------------------------------------------------------
void CPUImageFilter::amplify ( CPUImageFilter& mom, float level ) {

	float scalef = level / 128.0f;

	cvMul( mom.getCvImage(), mom.getCvImage(), cvImageTemp, scalef );
	swapTemp();
	flagImageChanged();
}

void CPUImageFilter::highpass ( float blur1, float blur2 ) {
	
	//Blur Original Image
	if(blur1 > 0)
	cvSmooth( cvImage, cvImageTemp, CV_BLUR , (blur1 * 2) + 1);

	//Original Image - Blur Image = Highpass Image
	cvSub( cvImage, cvImageTemp, cvImageTemp );

	//Blur Highpass to remove noise
	if(blur2 > 0)
	cvSmooth( cvImageTemp, cvImageTemp, CV_BLUR , (blur2 * 2) + 1);

	swapTemp();
	flagImageChanged();
}

void CPUImageFilter::invert ( bool inv ) {
	
	//Blur Original Image
	if(inv)	cvNot( cvImage, cvImageTemp );
	
	swapTemp();
	flagImageChanged();
}


//--------------------------------------------------------------------------------
void CPUImageFilter::operator =	( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void CPUImageFilter::operator = ( const ofxCvGrayscaleImage& _mom ) {
    if(this != &_mom) {  //check for self-assignment
        // cast non-const,  no worries, we will reverse any chages
        ofxCvGrayscaleImage& mom = const_cast<ofxCvGrayscaleImage&>(_mom); 
		
		ofRectangle iRoi = (*this).getIntersectionROI( (*this).getROI(), mom.getROI() );
		
		if( iRoi.width > 0 && iRoi.height > 0 ) {
        //if( pushSetBothToTheirIntersectionROI(*this,mom) ) {
			mom.setROI(iRoi);
			(*this).setROI(iRoi);
            cvCopy( mom.getCvImage(), cvImage, 0 );
			mom.resetROI(); //restore prevoius ROI
			(*this).resetROI();	//restore prevoius ROI
			
            //popROI();       //restore prevoius ROI
            //mom.popROI();   //restore prevoius ROI              
            flagImageChanged();
        } else {
            ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
        }
    } else {
        ofLog(OF_LOG_WARNING, "in =, you are assigning a ofxCvGrayscaleImage to itself");
    }
}

//--------------------------------------------------------------------------------
void CPUImageFilter::operator = ( const ofxCvColorImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvColorImage& mom = const_cast<ofxCvColorImage&>(_mom); 
	
	ofRectangle iRoi = (*this).getIntersectionROI( (*this).getROI(), mom.getROI() );
	
	if( iRoi.width > 0 && iRoi.height > 0 ) {
        //if( pushSetBothToTheirIntersectionROI(*this,mom) ) {
		mom.setROI(iRoi);
		(*this).setROI(iRoi);
		cvCvtColor( mom.getCvImage(), cvImage, CV_RGB2GRAY );
		mom.resetROI(); //restore prevoius ROI
		(*this).resetROI();	//restore prevoius ROI    
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}

//--------------------------------------------------------------------------------
void CPUImageFilter::operator = ( const ofxCvFloatImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvFloatImage& mom = const_cast<ofxCvFloatImage&>(_mom); 
	
	ofRectangle iRoi = (*this).getIntersectionROI( (*this).getROI(), mom.getROI() );
	
	if( iRoi.width > 0 && iRoi.height > 0 ) {
        //if( pushSetBothToTheirIntersectionROI(*this,mom) ) {
		mom.setROI(iRoi);
		(*this).setROI(iRoi);
		
		//cvConvertScale( mom.getCvImage(), cvImage, 1.0f, 0);
        cvConvert( mom.getCvImage(), cvImage );
		mom.resetROI(); //restore prevoius ROI
		(*this).resetROI();	//restore prevoius ROI          
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}

// ofxCvImage& mom, const ofPoint src[4], const ofPoint dst[4]


void CPUImageFilter::getQuadSubImage(unsigned char * inputData, unsigned char * outputData,  
									 int inW, int inH, int outW, int outH,  
									 const ofPoint dst[4], int bpp ) { 
	int x1 = dst[0].x;
	int y1 = dst[0].y;
	int x2 = dst[1].x;
	int y2 = dst[1].y;  
	int x3 = dst[2].x;
	int y3 = dst[2].y; 
	int x4 = dst[3].x;
	int y4 = dst[3].y;
	
	
    for(int x=0;x<outW;x++) {  
        for(int y=0;y<outH;y++) {  
            float xlrp = x/(float)outW;  
            float ylrp = y/(float)outH;  
            int xinput = (x1*(1-xlrp)+x2*xlrp)*(1-ylrp) + (x4*(1-xlrp)+x3*xlrp)*ylrp;  
            int yinput = (y1*(1-ylrp)+y4*ylrp)*(1-xlrp) + (y2*(1-ylrp)+y3*ylrp)*xlrp;  
            int inIndex = (xinput + yinput*inW)*bpp;  
            int outIndex = (x+y*outW)*bpp;  
            memcpy((void*)(outputData+outIndex),(void*)(inputData+inIndex),sizeof(unsigned char)*bpp);  
        }  
    }  
}  
