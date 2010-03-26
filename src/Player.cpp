/*
 *  Player.cpp
 *  openFrameworks
 *
 *  Created by Axel Esquite on 10/10/09.
 *  Copyright 2009 Axel Esquite. All rights reserved.
 *
 */

#include "Player.h"

Player::Player() {
	// TODO: Get this from XML SETTINGS
	mf_hit_points		= 100.0f;
	mf_player_radius	= 25.0f;
	//setPlayerPosition(ofGetScreenWidth()/2, ofGetScreenHeight()/2);
}

//------------------------------------------------------------
void Player::draw() {
	
	ofSetColor(214, 66, 3);
	ofCircle(mf_player_x, mf_player_y, mf_player_radius);
	ofSetColor(255, 255, 255);
	ofCircle(mf_player_x, mf_player_y, mf_player_radius/1.2);
	
}

//------------------------------------------------------------
bool Player::checkHit(ofxVec2f p2f_position, float f_radius) {
	
	float f_radius_length = f_radius + mf_player_radius;
	
	ofxVec2f distance = getPlayerPoint() - p2f_position;
	
	float f_total_length = distance.length();
	
	if(f_total_length < f_radius_length)
	{
		return true;
	}
	
	return false;
}

//------------------------------------------------------------
void Player::deliverHit(float f_damage) {
	
	mf_hit_points -= f_damage;
	
}

// SETTERS

//------------------------------------------------------------
void Player::setPlayerPosition(float f_x_pos, float f_y_pos) {
	mf_player_x = f_x_pos;
	mf_player_y = f_y_pos;
}

//------------------------------------------------------------
void Player::setPlayerPosition(int i_x_pos, int i_y_pos) {
	mf_player_x = (float) i_x_pos;
	mf_player_y = (float) i_y_pos;
}

//------------------------------------------------------------
void Player::setPlayerX(float f_x_pos) {
	mf_player_x = f_x_pos;
}

//------------------------------------------------------------
void Player::setPlayerY(float f_y_pos) {
	mf_player_y = f_y_pos;
}

//------------------------------------------------------------
void Player::setPlayerHitPoints(float f_life){
	mf_hit_points = f_life;
}
//------ Sets radius from where player can take damage -------
void Player::setDamageRadius(float f_damage_radius){
	mf_player_radius = f_damage_radius;
}

//------------------------------------------------------------
void Player::setGunPower(float f_gun_power){
	mf_gun_power = f_gun_power;
}



// GETTERS

//------------------------------------------------------------
float Player::getPlayerX() {
	return mf_player_x;
}

//------------------------------------------------------------
float Player::getPlayerY() {
	return mf_player_y;
}

//------------------------------------------------------------
ofxPoint2f Player::getPlayerPoint() {
	
	ofxPoint2f p_player_pos;
	
	p_player_pos.set(mf_player_x, mf_player_y);
	
	return p_player_pos;
}

//------------------------------------------------------------
bool Player::isAlive(){
	
	if(mf_hit_points <= 0 )
	{
		return false;
	}
	else
	{
		return true;
	}
}