/*
 *  Creature.h
 *  openFrameworks
 *
 *  Created by Axel Esquite on 10/10/09.
 *  Copyright 2009 Axel Esquite. All rights reserved.
 *
 */

#ifndef _CREATURE
#define _CREATURE

#include "ofMain.h"
#include "ofxVectorMath.h"


class Creature {
	
public:
	Creature();
	
	void draw();
	void resetForce();
	void addForce(float x, float y);
	
	void addRepulsionForce(ofxVec2f posOfForce, float radius, float scale);
	void addAttractionForce(float x, float y, float radius, float scale);
	
	void addDampingForce();
	
	void setInitialCondition(float px, float py, float vx, float vy);
	void setPosition(float px, float py);
	void setVelocity(float vx, float vy);
	void update();
	void die();
	
	void bounceOffWalls();
	bool checkHit(float f_gunX, float f_gunY, float f_gun_radius);
	bool checkHit(ofxVec2f ofxvec_gun_pos, float f_gun_radius);
	void loadSprite();
	void loadSounds();
	
	// SETTERS
	void setAllProperties(string s_creature_type, 
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
						  int i_frame_rate);
	
	void setAllProperties(Creature creature_archetype);
	
	// TODO: make setters for individual properties
	
	// GETTERS
	ofxVec2f getPosition();
	float getXPosition();
	float getYPosition();
	float getRadius();
	float getDamage();
	float getStrength();
	float getDamping();
	float getTooClose();
	float getInSight();
	float getScale();
	float getRepulsionRadius();
	float getHitPoints();
	bool getRepel();
	string getType();
	string getSprite();
	string getSounds();
	int getFrameRate();
	
	float randTurn;
	// TODO: get sprite should return png
	// TODO: get sound should return audio
	
protected:
	
	ofxVec2f ofv_pos;
	ofxVec2f ofv_vel;
	ofxVec2f ofv_frc;   // frc is also know as acceleration (newton says "f=ma")
	string ms_creature_type;
	string ms_sprite;
	string ms_sounds;
	bool mb_repel;
	float mf_damage;
	float mf_damping;
	float mf_radius;
	float mf_strength;
	float mf_too_close;
	float mf_in_sight;
	float mf_scale;
	float mf_repulsion_radius;
	float mf_hit_points;
	int mi_frame_rate;
	
	float mf_angle;
	float mf_theta;
	float mf_heading2D;
	int mi_frame_num;
	int mi_last_frame_number;
	
	std::vector<string> mv_images;
	std::vector<ofImage> mvimg_sprites;
};

#endif