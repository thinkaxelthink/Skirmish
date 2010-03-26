/*
 *  IRTracker.h
 *  openFrameworks
 *
 *  Created by Axel Esquite on 11/15/09.
 *  Copyright 2009 Axel Esquite. All rights reserved.
 *
 */


#ifndef _IR_TRACKER
#define _IR_TRACKER

#include "ofMain.h"
#include "ofxOpenCv.h"

class IRTracker {
	
public:
	
	IRTracker();
	void setupCamera(int i_deviceNumber);
	void setupCV();
	void processFrame();
	void drawFeed();
	void printPlayerBlobArea();
	
	float getPlayerBlobX();
	float getPlayerBlobY();
	
	float getBulletX();
	float getBulletY();
	
	void setSourcePoint(int i_point_adjusting, int i_x, int i_y);
		
protected:
	
	int i_width, i_height;
	bool b_cameraSetup, b_videoSetup;
	float f_playerBlobArea,f_bulletMinBlobArea, f_bulletMaxBlobArea;
	
	ofPoint ofpoint_srcPoints[4];
	ofPoint ofpoint_dstPoints[4];
	
	ofVideoGrabber			ofvidGrab_irFeed;
	ofxCvColorImage			ofxCvColor_irColorImage;
	ofxCvGrayscaleImage		ofxCvGray_irGrayImage;
	ofxCvGrayscaleImage		ofxCvGray_irGrayImagePreWarp;
	ofxCvGrayscaleImage		ofxCvGray_videoBgImage;
	
	ofxCvGrayscaleImage		ofxCvGray_irDiffImage;
	ofxCvContourFinder		ofxCvContourFind_contourFinder;
		
};

#endif
	