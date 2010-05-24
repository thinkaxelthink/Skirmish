/*
 *  IRTracker.cpp
 *  openFrameworks
 *
 *  Created by Axel Esquite on 11/15/09.
 *  Copyright 2009 Axel Esquite. All rights reserved.
 *
 */

#include "IRTracker.h"

IRTracker::IRTracker() {
	b_cameraSetup		= false;
	b_videoSetup		= false;
	
	// TODO: make this get dynamically set in getBulletPosition
	mf_bullet_radius	= 20.0f;
}

void IRTracker::setupCamera(int i_deviceNumber) {
	
	// setup device
	ofvidGrab_irFeed.setDeviceID(i_deviceNumber);
	
	// setup video grabber:
	ofvidGrab_irFeed.initGrabber(320, 240);
	
	// get the width and height, and allocate color and grayscale images: 
	i_width	= ofvidGrab_irFeed.width; 
	i_height = ofvidGrab_irFeed.height;		
}

//------------------------------------------------------------
void IRTracker::setupCV() {
	
	// setup video grabber:
	ofvidGrab_irFeed.initGrabber(320, 240);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	//ofvidGrab_irFeed.listDevices();
	//ofSetLogLevel(OF_LOG_WARNING);

	
	// get the width and height, and allocate color and grayscale images: 
	i_width = ofvidGrab_irFeed.width; 
	i_height = ofvidGrab_irFeed.height;
	
	ofxCvColor_irColorImage.allocate(i_width, i_height);
	ofxCvGray_irGrayImage.allocate(i_width, i_height);
	ofxCvGray_irGrayImagePreWarp.allocate(i_width, i_height);
	ofxCvGray_videoBgImage.allocate(i_width, i_height);
	ofxCvGray_irDiffImage.allocate(i_width, i_height);
	
	
	// set Default points
	// TODO: get from xml & always override before game starts
	ofpoint_srcPoints[0].set(0.0, 0.0);
	ofpoint_srcPoints[1].set(320.0, 0.0);
	ofpoint_srcPoints[2].set(320.0, 240.0);
	ofpoint_srcPoints[3].set(0.0, 240.0);
	
	ofpoint_dstPoints[0].set(0.0, 0.0);
	ofpoint_dstPoints[1].set(320.0, 0.0);
	ofpoint_dstPoints[2].set(320.0, 240.0);
	ofpoint_dstPoints[3].set(0.0, 240.0);
	
}

//------------------------------------------------------------
//process the incoming frame and look for a laser	
void IRTracker::processFrame() {
	
	///////////////////////////////////////////////////////////
	// Part 1 - get the video data
	///////////////////////////////////////////////////////////
	bool bLearnBg			= false;
	int threshold			= 79; //196
	int minBlobSize			= 25; 
	int maxBlobSize			= 1000;
	int nBlobsConsidered	= 2;
	
	
	
	ofvidGrab_irFeed.update();
	
	if (ofvidGrab_irFeed.isFrameNew()){
		
		ofxCvColor_irColorImage.setFromPixels(ofvidGrab_irFeed.getPixels(), i_width, i_height);
		ofxCvGray_irGrayImagePreWarp = ofxCvColor_irColorImage;
		ofxCvGray_irGrayImage.warpIntoMe(ofxCvGray_irGrayImagePreWarp, ofpoint_srcPoints, ofpoint_dstPoints);
		
		if (bLearnBg){ 
			ofxCvGray_videoBgImage = ofxCvGray_irGrayImage;
		}
		
		if (ofGetElapsedTimef() < 1.5){
			ofxCvGray_videoBgImage = ofxCvGray_irGrayImage;
		}
		
		ofxCvGray_irDiffImage.absDiff(ofxCvGray_irGrayImage, ofxCvGray_videoBgImage);
		ofxCvGray_irDiffImage.threshold(threshold);
		
		ofxCvContourFind_contourFinder.findContours(ofxCvGray_irDiffImage, minBlobSize, maxBlobSize, nBlobsConsidered, false, true);
		
	}
	
} 

//------------------------------------------------------------
void IRTracker::drawFeed() {
	
	ofxCvGray_irGrayImage.draw(20,20, 320,240);
	ofxCvGray_videoBgImage.draw(320+40, 20, 320, 240);
	ofxCvGray_irDiffImage.draw(20,240+40);
	ofxCvContourFind_contourFinder.draw(20,240+40);

}

//------------------------------------------------------------
void IRTracker::printAllBlobAreas() {
	
	// prints the area of all blob in nBlobs to the debugger.
	for(int i = 0; i < ofxCvContourFind_contourFinder.nBlobs; i++)
	{
		cout<<"blob area: "<<ofxCvContourFind_contourFinder.blobs[i].area<<endl;
	}
}

//------------------------------------------------------------
float IRTracker::getPlayerBlobX() {
	
	// Assumes a cv has been set up and frame processed

	float f_x_pos_returned;
	
	for (int i = 0; i <  ofxCvContourFind_contourFinder.blobs.size(); i++)
	{
		if(ofxCvContourFind_contourFinder.blobs[i].area >= f_playerMinBlobArea
			&& ofxCvContourFind_contourFinder.blobs[i].area <= f_playerMaxBlobArea)
		{
			f_x_pos_returned = ofxCvContourFind_contourFinder.blobs[i].centroid.x;
			
			// TODO: Use this for security when player leaves play field
			//mf_player_last_xpos = f_x_pos_returned;
		}
	}
	
	return f_x_pos_returned;
	
}

//------------------------------------------------------------
float IRTracker::getPlayerBlobY() {
	
	// Assumes a cv has been set up and frame processed
	
	float f_y_pos_returned;
	
	for (int i = 0; i <  ofxCvContourFind_contourFinder.blobs.size(); i++)
	{
		if(ofxCvContourFind_contourFinder.blobs[i].area >= f_playerMinBlobArea
		   && ofxCvContourFind_contourFinder.blobs[i].area <= f_playerMaxBlobArea)
		{
			f_y_pos_returned = ofxCvContourFind_contourFinder.blobs[i].centroid.y;
			
			// TODO: Use this for security when player leaves play field
			//mf_player_last_ypos = f_y_pos_returned;
		}
	}
	
	return f_y_pos_returned;
	
}

//------------------------------------------------------------
ofxVec2f IRTracker::getPlayerPosition() {
	
	ofxVec2f ofxvec_player;
	
	for (int i = 0; i <  ofxCvContourFind_contourFinder.blobs.size(); i++)
	{
		if(ofxCvContourFind_contourFinder.blobs[i].area >= f_playerMinBlobArea
		   && ofxCvContourFind_contourFinder.blobs[i].area <= f_playerMaxBlobArea)
		{
			ofxvec_current_player_position.set((float) (ofxCvContourFind_contourFinder.blobs[i].centroid.x * 3.2),
											   (float) (ofxCvContourFind_contourFinder.blobs[i].centroid.y * 3.2));
			ofxvec_player = (0.8 * ofxvec_last_player_position) + (0.2 * ofxvec_current_player_position);
			
			//mb_is_player = true;
			
			ofxvec_last_player_position.set((float) (ofxCvContourFind_contourFinder.blobs[i].centroid.x * 3.2),
											(float) (ofxCvContourFind_contourFinder.blobs[i].centroid.y * 3.2));
			
		}
		/*else
		 {
		 //mb_is_player = false;
		 }*/
	}
	
	return ofxvec_player;
}

//------------------------------------------------------------
float IRTracker::getBulletX() {
	
	float f_bullet_x;
	
	for(int i = 0; i < ofxCvContourFind_contourFinder.blobs.size(); i++)
	{
		if(ofxCvContourFind_contourFinder.blobs[i].area >= f_bulletMinBlobArea
			&& ofxCvContourFind_contourFinder.blobs[i].area <= f_bulletMaxBlobArea)
		{
			f_bullet_x = ofxCvContourFind_contourFinder.blobs[i].centroid.x;
			//cout<<"found bullet X: "<<f_bullet_x<<endl;
		}
	}
	
	return f_bullet_x;
}

//------------------------------------------------------------
float IRTracker::getBulletY() {
	
	float f_bullet_y;
	
	for(int i = 0; i < ofxCvContourFind_contourFinder.blobs.size(); i++)
	{
		if(ofxCvContourFind_contourFinder.blobs[i].area >= f_bulletMinBlobArea
			&& ofxCvContourFind_contourFinder.blobs[i].area <= f_bulletMaxBlobArea)
		{
			f_bullet_y = ofxCvContourFind_contourFinder.blobs[i].centroid.y;
			//cout<<"found bullet Y: "<<f_bullet_y<<endl;
		}
	}
	
	return f_bullet_y;
}

//------------------------------------------------------------
ofxVec2f IRTracker::getBulletPosition() {
	
	ofxVec2f ofxvec_bullet;
	
	ofxvec_bullet.set((float) (getBulletX() * 3.2), (float) (getBulletY() * 3.2));
	
	return ofxvec_bullet;
}

//------------------------------------------------------------
float IRTracker::getBulletRadius() {
	return mf_bullet_radius;
}

//------------------------------------------------------------
void IRTracker::setPlayerBlobArea(float f_min_area, float f_max_area) {
	f_playerMinBlobArea = f_min_area;
	f_playerMaxBlobArea = f_max_area;
}

//------------------------------------------------------------
void IRTracker::setBulletBlobArea(float f_min_area, float f_max_area) {
	f_bulletMinBlobArea = f_min_area;
	f_bulletMaxBlobArea = f_max_area;
}

//------------------------------------------------------------
void IRTracker::setSourcePoint(int i_point_adjusting, int i_x, int i_y) {
	ofpoint_srcPoints[i_point_adjusting].set((float) i_x, (float) i_y);
	cout<<"Params are: "<<i_x<<", "<<i_y <<"for pt - "<<i_point_adjusting<<endl;
	cout<<"Source points after set - X: "<<ofpoint_srcPoints[i_point_adjusting].x<<", Y: "<<ofpoint_srcPoints[i_point_adjusting].y<<endl;
}