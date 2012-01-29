/*
 *  ofxTSPSPerson.cpp
 *  peopleTracker_000
 *
 *  Adapted by James George on 11/21/09.
 *  Copyright 2009 (http://www.jamesgeorge.org) All rights reserved.
 *
 */
#include "ofxTSPSPerson.h"

ofxTSPSPerson::ofxTSPSPerson(int pid, int oid, ofxCvBlob blob)
: pid(pid),
  oid(oid),
  age(0),
  opticalFlowVectorAccumulation(ofPoint(0,0)),
  centroid(blob.centroid),
  customAttributes(NULL)
{
	update(blob, false);
}

ofxTSPSPerson::~ofxTSPSPerson()
{
	if(customAttributes != NULL){
		free(customAttributes);
	}
}

void ofxTSPSPerson::update(ofxCvBlob blob, bool dampen)
{
	if(dampen){
		centroid = (centroid * .7) + (blob.centroid * .3);
	}
	else{
		centroid = blob.centroid;
	}
	
	velocity	 = blob.centroid - centroid;
	area		 = blob.area;
	boundingRect = blob.boundingRect;
	contour		 = blob.pts; 
	age++;
}


ofRectangle ofxTSPSPerson::getBoundingRectNormalized(float videoWidth, float videoHeight)
{
	return ofRectangle( ofNormalize( boundingRect.x, 0.0, videoWidth),
						ofNormalize( boundingRect.y, 0.0, videoHeight),
						ofNormalize( boundingRect.width, 0.0, videoWidth),
						ofNormalize( boundingRect.height, 0.0, videoHeight)
					    );
}


ofPoint ofxTSPSPerson::getCentroidNormalized(float videoWidth, float videoHeight)
{
	return ofPoint(ofMap( centroid.x / videoWidth,  0.0, 1.0, -1.0, 1.0 ),
				   ofMap( centroid.y / videoHeight,  0.0, 1.0, -1.0, 1.0 )
				   );
}
