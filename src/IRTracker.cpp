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
	f_playerBlobArea	= 300.0f; // 100.0f
	f_bulletMinBlobArea	= 100.0f;
	f_bulletMaxBlobArea	= 200.0f;
	
	
	//int playerBlobAreaMin	= 500;
	//int playerBlobAreaMax	= 1500;
	
	//int gunBlobAreaMin		= 30;
	//int gunBlobAreaMax		= 300;
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
		
		//printf("we found %i blobs \n", ofxCvContourFind_contourFinder.nBlobs);
		/*for (int i = 0; i <  ofxCvContourFind_contourFinder.nBlobs; i++){
			//printf("centroid of blob %i = (%f,%f) \n", i, ofxCvContourFind_contourFinder.blobs[i].centroid.x, ofxCvContourFind_contourFinder.blobs[i].centroid.y);
			printf("area of blob %i = (%f,%f) \n", i, ofxCvContourFind_contourFinder.blobs[i].area);
		}*/
	}
	
} 

void IRTracker::drawFeed() {
	
	ofxCvGray_irGrayImage.draw(20,20, 320,240);
	ofxCvGray_videoBgImage.draw(320+40, 20, 320, 240);
	ofxCvGray_irDiffImage.draw(20,240+40);
	ofxCvContourFind_contourFinder.draw(20,240+40);

}

void IRTracker::printPlayerBlobArea() {
	
	// Bad name for this. Function returns the size of the blob of
	// my helmet light so that i can average out how big they player can be.
	for(int i = 0; i < ofxCvContourFind_contourFinder.nBlobs; i++)
	{
		printf("%f \n", ofxCvContourFind_contourFinder.blobs[i].area);
	}
}

float IRTracker::getPlayerBlobX() {
	
	// Assumes a cv has been set up and frame processed
	
	float f_x_pos_returned;
	
	for (int i = 0; i <  ofxCvContourFind_contourFinder.blobs.size(); i++)
	{
		if(ofxCvContourFind_contourFinder.blobs[i].area >= f_playerBlobArea)
		{
			f_x_pos_returned = ofxCvContourFind_contourFinder.blobs[i].centroid.x;
		}
		else
		{
			// TODO: store old position so that if blob is missing/cannot be detected
			// you will have the last position
			f_x_pos_returned = 0;
		}
	}
	
	return f_x_pos_returned;
	
}

float IRTracker::getPlayerBlobY() {
	
	// Assumes a cv has been set up and frame processed
	
	float f_y_pos_returned;
	
	for (int i = 0; i <  ofxCvContourFind_contourFinder.blobs.size(); i++)
	{
		if(ofxCvContourFind_contourFinder.blobs[i].area >= f_playerBlobArea)
		{
			f_y_pos_returned = ofxCvContourFind_contourFinder.blobs[i].centroid.y;
		}
		else
		{
			// TODO: store old position so that if blob is missing/cannot be detected
			// you will have the last position
			f_y_pos_returned = 240.0f;
		}
	}
	
	return f_y_pos_returned;
	
}

float IRTracker::getBulletX() {
	
	float f_bullet_x;
	
	for(int i = 0; i < ofxCvContourFind_contourFinder.blobs.size(); i++)
	{
		if(ofxCvContourFind_contourFinder.blobs[i].area >= f_bulletMinBlobArea
			&& ofxCvContourFind_contourFinder.blobs[i].area <= f_bulletMaxBlobArea)
		{
			f_bullet_x = ofxCvContourFind_contourFinder.blobs[i].centroid.x;
		}
		else
		{
			// TODO: Can this just return NULL? if so awesome. 0 is still a value and if a 
			// creature happens to be at 0,0 it will die and that's not really fair
			f_bullet_x = 0.0f;
		}
	}
	
	return f_bullet_x;
}

float IRTracker::getBulletY() {
	
	float f_bullet_y;
	
	for(int i = 0; i < ofxCvContourFind_contourFinder.blobs.size(); i++)
	{
		if(ofxCvContourFind_contourFinder.blobs[i].area >= f_bulletMinBlobArea
			&& ofxCvContourFind_contourFinder.blobs[i].area <= f_bulletMaxBlobArea)
		{
			f_bullet_y = ofxCvContourFind_contourFinder.blobs[i].centroid.y;
		}
		else
		{
			// TODO: Can this just return NULL? if so awesome. 0 is still a value and if a 
			// creature happens to be at 0,0 it will die and that's not really fair
			f_bullet_y = 0.0f;
		}
	}
	
	return f_bullet_y;
}

void IRTracker::setSourcePoint(int i_point_adjusting, int i_x, int i_y) {
	ofpoint_srcPoints[i_point_adjusting].set((float) i_x, (float) i_y);
	printf("Params are: %d, %d for pt - %d \n", i_x, i_y, i_point_adjusting);
	printf("Source points after set: %f, %f \n", ofpoint_srcPoints[i_point_adjusting].x, ofpoint_srcPoints[i_point_adjusting].y);
}