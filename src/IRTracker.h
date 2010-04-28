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
#include "ofxVectorMath.h"

class IRTracker {
	
public:
	
	IRTracker();
	void setupCamera(int i_deviceNumber);
	void setupCV();
	void processFrame();
	void drawFeed();
	void printAllBlobAreas();
	
	float getPlayerBlobX();
	float getPlayerBlobY();
	
	float getBulletX();
	float getBulletY();
	ofxVec2f getBulletPosition();
	float getBulletRadius();
	
	void setPlayerBlobArea(float f_min_area, float f_max_area);
	void setBulletBlobArea(float f_min_area, float f_max_area);
	void setSourcePoint(int i_point_adjusting, int i_x, int i_y);
		
protected:
	
	int i_width, i_height;
	bool b_cameraSetup, b_videoSetup;
	float f_playerBlobArea,f_playerMinBlobArea,f_playerMaxBlobArea,f_bulletMinBlobArea, f_bulletMaxBlobArea;
	float mf_player_last_xpos, mf_player_last_ypos;
	float mf_bullet_radius;
	
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
	