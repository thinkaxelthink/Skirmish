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
	//setInitialCondition(0, 0, 0, 0);
	//mf_damping = 0.4f;
	//mf_radius = 15.0f;
	//mf_damage = 2.0f;
	mi_frame_num = 0;
	mi_last_frame_number = 0;
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
	
	mf_angle = (float)atan2(-ofv_vel.y, ofv_vel.x);
    mf_theta =  -1.0 * mf_angle;
	mf_heading2D = ofRadToDeg(mf_theta)+90;	
	
	// get frame number
	// draw image at that index
	// save frame num as last_frame_num
	int i_current_frame = ofGetFrameNum();
	
	// check if this is a new frame
	if(mi_last_frame_number != i_current_frame)
	{
		// This sets up our frame rate
		// any frame divisible by our frame rate (19) 
		// will make the sprite play from the begining
		if(i_current_frame %mi_frame_rate == 0)
		{
			mi_frame_num = 0;
		}
		else
		{
			// otherwise increment mvimg_sprites index by 1
			mi_frame_num++;
		}
		
		mi_last_frame_number = i_current_frame;
	}
}

//------------------------------------------------------------
void Creature::draw(){

	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255);
	
	ofPushMatrix();
    ofTranslate(ofv_pos.x, ofv_pos.y);
    ofRotateZ(mf_heading2D);	
	
	mvimg_sprites[mi_frame_num].setAnchorPercent(0.5,0.5);
	mvimg_sprites[mi_frame_num].draw(0,0);

    ofPopMatrix();
	
	ofDisableAlphaBlending();
	ofRect(ofv_pos.x, ofv_pos.y, 100.0, 100.0);
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
	} 
	else if (ofv_pos.x < minx)
	{
		ofv_pos.x = minx; // move to the edge, (important!)
		ofv_vel.x *= -1;
		bDidICollide = true;
	}
	
	if (ofv_pos.y > maxy)
	{
		ofv_pos.y = maxy; // move to the edge, (important!)
		ofv_vel.y *= -1;
		bDidICollide = true;
	} 
	else if (ofv_pos.y < miny)
	{
		ofv_pos.y = miny; // move to the edge, (important!)
		ofv_vel.y *= -1;
		bDidICollide = true;
	}
	
	if (bDidICollide == true && bDampedOnCollision == true)
	{
		ofv_vel *= 0.3;
	}
	
}

//------------------------------------------------------------
bool Creature::checkHit(float f_gunX, float f_gunY){
	
	// TODO: check radius for % damage capability
	// or check radius, then check if it's a direct hit, 
	// apply damage percent accordingly
	if(f_gunX >= ofv_pos.x 
	   && f_gunX <= ofv_pos.x + 100.0
	   && f_gunY >= ofv_pos.y
	   && f_gunY <= ofv_pos.y + 100.0)
	{
		cout<<"checkHit returns true"<<endl;
		return true;
	}
	else
	{
		return false;
	}
}

//------------------------------------------------------------
void Creature::loadSprite() {
	// Go through all our image paths from xml
	for(int i = 0; i < mv_images.size(); i++)
	{
		// create a temp ofImage for load
		ofImage temp_image;
		
		// load image from path at mv_image index
		temp_image.loadImage(mv_images[i]);
		/*if(temp_image.loadImage(mv_images[i]))
		{
			cout<<mv_images[i]<<" was loaded."<<endl;
		}
		else
		{
			cout<<mv_images[i]<<" could not be loaded"<<endl;
		}*/
		
		// store it for use later in this vector
		mvimg_sprites.push_back(temp_image);
	}
}

//------------------------------------------------------------
void Creature::loadSounds() {
	//msound_sound.loadSound(s_sounds, false);
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
								float f_hit_points,
								int	i_frame_rate) {
	
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
	mi_frame_rate		= i_frame_rate;
	
	string temp_sprite;
	stringstream stream(ms_sprite);
	while(getline(stream, temp_sprite, ','))
	{
		mv_images.push_back(temp_sprite);
	}
	
	cout<<"creature name: "<<ms_creature_type<<endl;
	cout<<"creature fps: "<<mi_frame_rate<<endl;
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
	mi_frame_rate		= creature_archetype.getFrameRate();
	
	string temp_sprite;
	stringstream stream(ms_sprite);
	while(getline(stream, temp_sprite, ','))
	{
		mv_images.push_back(temp_sprite);
	}
	
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

//------------------------------------------------------------
int Creature::getFrameRate() {
	return mi_frame_rate;
}
