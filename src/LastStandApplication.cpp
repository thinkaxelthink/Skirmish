/*
 *  LastStandApplication.cpp
 *  openFrameworks
 *
 *  Created by Axel Esquite on 10/10/09.
 *  Copyright 2009 Axel Esquite. All rights reserved.
 *
 */

#include "LastStandApplication.h"
using namespace std;

//--------------------------------------------------------------
void LastStandApplication::setup(){
	
	/********************************************
	 ** LOAD SETTINGS FROM XML
	 ********************************************/
	//hitTime = 255;
	// check if load was successful
	setHitTime();
	if( XML.loadFile("data.xml") ){
		
		cout<<"data.xml loaded! \n"<<endl;
		
		// It's all gravy - XML loaded now disperse data
		// first load Game Settings, use GAME_SETTINGS as ROOT
		XML.pushTag("GAME_SETTINGS", 0);
			
			// core game
			player_max				= XML.getValue("PLAYER_MAX", 0);	
			frame_speed				= XML.getValue("FRAME_SPEED", 50);
			play_field.loadImage(XML.getValue("PLAYFIELD", "DEFAULT"));
			
			// for game score, but need more params
			gameScore.setFormat(XML.getValue("SCORE_FORMAT", "TIME"), 0, "DIN-Regular.ttf", 30);
			
			// player settings
			player.setPlayerHitPoints(XML.getValue("PLAYER_SETTINGS:HIT_POINTS", 0.0f));
			player.setDamageRadius(XML.getValue("PLAYER_SETTINGS:DAMAGE_RADIUS", 0.0f));
			player.setGunPower(XML.getValue("PLAYER_SETTINGS:GUN_POWER", 0.0f));
			camera.setPlayerBlobArea(XML.getValue("PLAYER_SETTINGS:PLAYER_MIN_BLOB_AREA", 0.0f), 
									 XML.getValue("PLAYER_SETTINGS:PLAYER_MAX_BLOB_AREA", 0.0f));
			camera.setBulletBlobArea(XML.getValue("PLAYER_SETTINGS:GUN_MIN_BLOB_AREA", 0.0f), 
								 XML.getValue("PLAYER_SETTINGS:GUN_MAX_BLOB_AREA", 0.0f));
			// creature settings
			int numCreatureTags	= XML.getNumTags("CREATURE");
			// if there is at least one creature save the settings
			if(numCreatureTags > 0)
			{
				// loop through all of the creatures
				for(int i = 0; i < numCreatureTags; i++)
				{
					XML.pushTag("CREATURE", i);
					
					
					// sets up strength and area that creatures will flock to
					// TODO: this will have to change. creatures will take in player
					// as target and not need to have a radius, just track player
					//b_repel		= true;
					//f_radius	= 1000.0f;
					//f_strength	= 0.2f;
					
					// creature flock distance info :: are these global
					//f_too_close_dist	= 2.0f;
					//f_in_sight_distance = 6.0f;
					
					
					creatureFactory.setNewCreatureArchetype(XML.getValue("NAME", "DEFAULT"), 
															XML.getValue("DAMPING", 0.0f), 
															XML.getValue("REPEL", true), 
															XML.getValue("RADIUS", 0.0f), 
															XML.getValue("STRENGTH", 0.0f),
															XML.getValue("TOO_CLOSE_DISTANCE", 2.0f),
															XML.getValue("IN_SIGHT_DISTANCE", 6.0F),
															XML.getValue("SCALE", 0.2f),
															XML.getValue("REPULSION_RADIUS", 1000.0f),
															XML.getValue("SPRITE", "DEFAULT"),
															XML.getValue("SOUNDS", "DEFAULT"),
															XML.getValue("DAMAGE_RATE", 10.0f),
															XML.getValue("HIT_POINTS", 10.0f),
															XML.getValue("FRAME_RATE", 0));
					
					XML.popTag();
				}
				
			}
		
		// End of GAME_SETTINGS as ROOT
		XML.popTag();
		
	}else{
		printf("unable to load data.xml check data/ folder \n");
	}
	
	
	
	
	// TODO: possibly create a GUI class that handles menus
	// rendering bg and sprites?
	ofBackground(255,255,255);
	
	// TODO: get rid of this when sprites are in
	ofEnableSmoothing();
	ofSetCircleResolution(30);
	
	// TODO: define this in XML
	ofSetFrameRate(frame_speed);
	
	screen_width = ofGetScreenWidth();
	screen_height = ofGetScreenHeight();
	
	player.setPlayerPosition(ofGetWidth()/2, ofGetHeight()/2);
	camera.setupCV();
	
	#ifdef NO_CAMERA_MODE
	b_start_game			= true; 
	b_agreement_accepted	= true; 
	#else
	b_start_game			= false; 
	b_agreement_accepted	= false; 
	#endif
	b_adjusting_point		= false;
	i_which_point_adjusting	= 0;
	
	ofimg_caution.loadImage("images/CautionWhole.png");
	i_agreement_hit_count = 0;
}

//--------------------------------------------------------------
void LastStandApplication::update(){
	
	if(b_agreement_accepted == true && b_start_game == true)
	{
		if(player.isAlive() == false)
		{
			b_start_game		  = false;
			b_agreement_accepted  = false;
			i_start_time		  = 0;
			i_agreement_hit_count = 0;
			player.resetLife();
			creatureFactory.resetCreatures();
		}
		else
		{	
			// update timer
			i_timer = ofGetElapsedTimeMillis() - i_start_time;
			i_timer /= 1000;
			
			// update score type TIMER in seconds
			gameScore.update(i_timer);
			
			if(i_timer %5 == 0)
			{
				// sets max creatures allowed on the field
				creature_max = i_timer / 5;
			}
			
			// Finds which edge the player is closest to and sets factory's 
			// member x & y pos to that position
			//creatureFactory.findEdgeToSpawn); 
			
			// this will set max creatures allowed on the field
			// then calls spawnCreatures() internally
			creatureFactory.updateCreatureMax(creature_max, player.getPlayerX(), player.getPlayerY());
			
			// updates creature flock 
			creatureFactory.updateCreaturesFlock(player);
			
			
			//b_start_game = player.isAlive();
			
			// looks for ir blobs
			camera.processFrame();

			// Assumes there is always a blob
			// sets position of player to blob's (if blob is big enough to be registered as a player)
			// TODO: overload this to accept a 2d vec and make camera return 2d vecs
			#ifdef NO_CAMERA_MODE
			player.setPlayerPosition((float)mouseX, (float)mouseY);
			#else
			player.setPlayerPosition((float) (camera.getPlayerBlobX() * 3.2), (float) (camera.getPlayerBlobY() * 3.2));
			#endif
			creatureFactory.checkBulletPosition(camera.getBulletPosition(), camera.getBulletRadius());
			//creatureFactory.checkBulletPosition((float) (camera.getBulletX() * 3.2), (float) (camera.getBulletY() * 3.2));
		}
	}
	else
	{
		// looks for ir blobs
		camera.processFrame();
			
		if(i_agreement_hit_count < 3)
		{
			if(checkHit(camera.getBulletPosition(), camera.getBulletRadius(), 387.0f, 436.0f, 65.0f))
			{
				i_agreement_hit_count++;
				
			}
		}
		else
		{
			b_start_game		 = true;
			b_agreement_accepted = true;
			i_start_time = ofGetElapsedTimeMillis();
		}
	}
}

//--------------------------------------------------------------
void LastStandApplication::draw(){
	
	if(b_agreement_accepted == true && b_start_game == true)
	{
		ofSetColor(255, 255, 255);
		play_field.draw(0,0);
		// draw player
		player.draw();
		
		// draws all camera views
		//camera.drawFeed();
		// your score
		gameScore.outputAsString(730, 40);
		
		// if game is started
		if(b_start_game == true)
		{
			// draw creature flock
			creatureFactory.drawCreatures(player);
		}
		
		//draw hit bg
		checkPlayerHit(player);
		
		ofNoFill();
		ofCircle((camera.getBulletX() * 3.2), (camera.getBulletY() * 3.2), 5);
		ofFill();
	}
	else
	{
		ofSetColor(255, 255, 255);
		ofimg_caution.draw(0,0);
		/*ofNoFill();
		ofCircle((camera.getBulletX() * 3.2), (camera.getBulletY() * 3.2), 5);
		ofFill();*/
	}
}

//--------------------------------------------------------------
void LastStandApplication::keyPressed(int key){
	
	// check if we're setting warp points
	// check if game is starting
	switch (key){
		case '1':
			b_adjusting_point = true;
			i_which_point_adjusting = 0;
			break;
		case '2':
			b_adjusting_point = true;
			i_which_point_adjusting = 1;
			break;
		case '3':
			b_adjusting_point = true;
			i_which_point_adjusting = 2;
			break;
		case '4':
			b_adjusting_point = true;
			i_which_point_adjusting = 3;
			break;
		case 'x':
			b_adjusting_point = false;
			break;
		case 'q':
			b_start_game = true;
			i_start_time = ofGetElapsedTimeMillis();
			break;
	}
	
}

//--------------------------------------------------------------
void LastStandApplication::keyReleased(int key){
	
}

//--------------------------------------------------------------
void LastStandApplication::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void LastStandApplication::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void LastStandApplication::mousePressed(int x, int y, int button){
		
}

//--------------------------------------------------------------
void LastStandApplication::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void LastStandApplication::windowResized(int w, int h){
	
}

