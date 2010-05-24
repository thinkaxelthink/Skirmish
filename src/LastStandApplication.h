/*
 *  LastStandApplication.h
 *  openFrameworks
 *
 *  Created by Axel Esquite on 10/10/09.
 *  Copyright 2009 Axel Esquite. All rights reserved.
 *
 */

#ifndef _LAST_STAND_APPLICATION
#define _LAST_STAND_APPLICATION

// OF
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"

// App
#include "CreatureFactory.h"
#include "Player.h"
#include "GameScore.h"
#include "IRTracker.h"
#include "GameUtils.h"


class LastStandApplication : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	// XML & GAME CORE VARS
	ofxXmlSettings XML;
	int player_max;
	int frame_speed;
	ofImage play_field;
	
	// this variable determines how many creatures can appear on screen at a time
	int creature_max;
	int screen_width;
	int screen_height;
	
	Player				player;
	CreatureFactory		creatureFactory;
	IRTracker			camera;
	
	//Score
	GameScore			gameScore;
	
	bool	b_repel;
	float	f_radius;
	float	f_strength;
	//float   f_too_close_dist;
	//float   f_in_sight_distance;
	
	// timer for creature spawn
	int i_timer;
	int i_start_time;
	
	bool b_start_game;
	bool b_agreement_accepted;
	bool b_adjusting_point;
	int i_which_point_adjusting;
	int i_agreement_hit_count;
	
	ofImage ofimg_caution;
	
};

#endif
