/*
 *  Creature.cpp
 *  openFrameworks
 *
 *  Created by Axel Esquite on 10/10/09.
 *  Copyright 2009 Axel Esquite. All rights reserved.
 *
 */

#include "Creature.h"

Creature::Creature() {
	setInitialCondition(0, 0, 0, 0);
	mf_damping = 0.4f;
	mf_radius = 15.0f;
	mf_damage = 2.0f;
}

//------------------------------------------------------------
void Creature::resetForce(){
    // we reset the forces every frame
    ofv_frc.set(0,0);
}

//------------------------------------------------------------
void Creature::addForce(float x, float y){
    // add in a force in X and Y for this frame.
    ofv_frc.x = ofv_frc.x + x;
    ofv_frc.y = ofv_frc.y + y;
}

void Creature::addRepulsionForce(ofxVec2f posOfForce, float radius, float scale){
    
	// ----------- (1) make a vector of where this position is: 
	
	//ofxVec2f posOfForce;
	//posOfForce.set(x,y);
	
	// ----------- (2) calculate the difference & length 
	
	ofxVec2f diff	= ofv_pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
        diff.normalize();
		ofv_frc.x = ofv_frc.x + diff.x * scale * pct;
        ofv_frc.y = ofv_frc.y + diff.y * scale * pct;
    }
}

void Creature::addAttractionForce(float x, float y, float radius, float scale){
    
	// ----------- (1) make a vector of where this position is: 
	
	ofxVec2f posOfForce;
	posOfForce.set(x,y);
	
	// ----------- (2) calculate the difference & length 
	
	ofxVec2f diff	= ofv_pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		ofv_frc.x = ofv_frc.x - diff.x; //* scale * pct;
        ofv_frc.y = ofv_frc.y - diff.y; //* scale * pct;
    }
}


//------------------------------------------------------------
void Creature::addDampingForce(){
	
	// the usual way to write this is  vel *= 0.99
	// basically, subtract some part of the velocity 
	// damping is a force operating in the oposite direction of the 
	// velocity vector
	
    ofv_frc.x = ofv_frc.x - ofv_vel.x * mf_damping;
    ofv_frc.y = ofv_frc.y - ofv_vel.y * mf_damping;
}

//------------------------------------------------------------
void Creature::setInitialCondition(float px, float py, float vx, float vy){
    ofv_pos.set(px,py);
	ofv_vel.set(vx,vy);
}

//------------------------------------------------------------
void Creature::update(){	
	ofv_vel = ofv_vel + ofv_frc;
	ofv_pos = ofv_pos + ofv_vel;
}

//------------------------------------------------------------
void Creature::draw(){
	ofSetColor(195, 223, 234);
	ofFill();
    ofRect(ofv_pos.x, ofv_pos.y, 20, 20);
}

//------------------------------------------------------------
void Creature::die(){
	// delete this creature
}

//------------------------------------------------------------
void Creature::bounceOffWalls(){
	
	// sometimes it makes sense to damped, when we hit
	bool bDampedOnCollision = true;
	bool bDidICollide = false;
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
	
	if (ofv_pos.x > maxx){
		ofv_pos.x = maxx; // move to the edge, (important!)
		ofv_vel.x *= -1;
		bDidICollide = true;
	} else if (ofv_pos.x < minx){
		ofv_pos.x = minx; // move to the edge, (important!)
		ofv_vel.x *= -1;
		bDidICollide = true;
	}
	
	if (ofv_pos.y > maxy){
		ofv_pos.y = maxy; // move to the edge, (important!)
		ofv_vel.y *= -1;
		bDidICollide = true;
	} else if (ofv_pos.y < miny){
		ofv_pos.y = miny; // move to the edge, (important!)
		ofv_vel.y *= -1;
		bDidICollide = true;
	}
	
	if (bDidICollide == true && bDampedOnCollision == true){
		ofv_vel *= 0.3;
	}
	
}

//------------------------------------------------------------
bool Creature::checkHit(float f_gunX, float f_gunY){
	
	// TODO: check radius for % damage capability
	// or check radius, then check if it's a direct hit, 
	// apply damage percent accordingly
	if(f_gunX >= ofv_pos.x 
	   && f_gunX <= ofv_pos.x + 20.0
	   && f_gunY >= ofv_pos.y
	   && f_gunY <= ofv_pos.y + 20.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//------------------------------------------------------------
void Creature::setAllProperties(string s_creature_type, 
								float f_damping, 
								bool b_repel, 
								float f_radius, 
								float f_strength, 
								float f_too_close, 
								float f_in_sight,
								float f_scale,
								float f_repulsion_radius,
								string s_sprite,
								string s_sounds,
								float f_damage_rate,
								float f_hit_points) {
	
	ms_creature_type	= s_creature_type;
	mf_damping			= f_damping;
	mb_repel			= b_repel;
	mf_radius			= f_radius;
	mf_strength			= f_strength;
	mf_too_close		= f_too_close;
	mf_in_sight			= f_in_sight;
	mf_scale			= f_scale;
	mf_repulsion_radius = f_repulsion_radius;
	ms_sprite			= s_sprite;
	ms_sounds			= s_sounds;
	mf_damage			= f_damage_rate;
	mf_hit_points		= f_hit_points;
}

//------------------------------------------------------------
void Creature::setAllProperties(Creature creature_archetype) {
	
	ms_creature_type	= creature_archetype.getType();
	mf_damping			= creature_archetype.getDamping();
	mb_repel			= creature_archetype.getRepel();
	mf_radius			= creature_archetype.getRadius();
	mf_strength			= creature_archetype.getStrength();
	mf_too_close		= creature_archetype.getTooClose();
	mf_in_sight			= creature_archetype.getInSight();
	mf_scale			= creature_archetype.getScale();
	mf_repulsion_radius = creature_archetype.getRepulsionRadius();
	ms_sprite			= creature_archetype.getSprite();
	ms_sounds			= creature_archetype.getSounds();
	mf_damage			= creature_archetype.getDamage();
	mf_hit_points		= creature_archetype.getHitPoints();
	
}

//------------------------------------------------------------
ofxVec2f Creature::getPosition() {
	return ofv_pos;
}

//------------------------------------------------------------
float Creature::getXPosition() {
	return ofv_pos.x;
}

//------------------------------------------------------------
float Creature::getYPosition() {
	return ofv_pos.y;
}

//------------------------------------------------------------
float Creature::getRadius() {
	return mf_radius;
}

//------------------------------------------------------------
float Creature::getDamage() {
	return mf_damage;
}

//------------------------------------------------------------
float Creature::getStrength() {
	return mf_strength;
}

//------------------------------------------------------------
float Creature::getDamping() {
	return mf_damping;
}

//------------------------------------------------------------
float Creature::getTooClose() {
	return mf_too_close;
}

//------------------------------------------------------------
float Creature::getInSight() {
	return mf_in_sight;
}

//------------------------------------------------------------
float Creature::getScale() {
	return mf_scale;
}

//------------------------------------------------------------
float Creature::getRepulsionRadius() {
	return mf_repulsion_radius;
}

//------------------------------------------------------------
float Creature::getHitPoints() {
	return mf_hit_points;
}

//------------------------------------------------------------
string Creature::getType() {
	return ms_creature_type;
}

//------------------------------------------------------------
bool Creature::getRepel() {
	return mb_repel;
}

//------------------------------------------------------------
string Creature::getSprite() {
	return ms_sprite;
}

//------------------------------------------------------------
string Creature::getSounds() {
	return ms_sounds;
}
