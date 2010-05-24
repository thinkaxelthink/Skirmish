/*
 *  Player.h
 *  openFrameworks
 *
 *  Created by Axel Esquite	on 10/10/09.
 *  Copyright 2009 Esquite. All rights reserved.
 *
 */

#ifndef _PLAYER
#define _PLAYER

#include "ofMain.h"
#include "ofxVectorMath.h"


class Player {
	
public:
	Player();	
	
	// METHODS
	void draw();
	bool checkHit(ofxVec2f p2f_position, float f_radius);
	void deliverHit(float f_damage);
	void resetLife();
	
	// SETTERS
	void setPlayerPosition(float f_x_pos, float f_y_pos);
	void setPlayerPosition(int i_x_pos, int i_y_pos);
	void setPlayerX(float f_x_pos);
	void setPlayerY(float f_y_pos);
	void setPlayerHitPoints(float f_life);
	void setDamageRadius(float f_damage_radius);
	void setGunPower(float f_gun_power);
	
	// GETTERS
	float getPlayerX();
	float getPlayerY();
	ofxPoint2f getPlayerPoint();
	bool isAlive();
	
protected:
	
	float mf_player_radius;
	float mf_hit_points;
	float mf_hit_point_reset;
	float mf_gun_power;
	float mf_player_x;
	float mf_player_y;
	
};

#endif