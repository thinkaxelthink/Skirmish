/*
 *  CreatureFactory.h
 *  openFrameworks
 *
 *  Created by Axel Esquite on 10/10/09.
 *  Copyright 2009 Axel Esquite. All rights reserved.
 *
 */

#ifndef _CREATURE_FACTORY
#define _CREATURE_FACTORY

#include "ofMain.h"
#include "Creature.h"
#include "Player.h"

#include <vector>

class CreatureFactory : public Creature {
	
public:
	
	CreatureFactory();
	void startFactory(bool b_repel, float f_radius, float f_strength, float f_too_close_dist, float f_in_sight_distance); 
	void updateCreatureMax(int i_creature_max, float f_player_x, float f_player_y );
	void findEdgeToSpawn(float f_player_x, float f_player_y);
	void spawnCreatures();
	void resetCreatures();
	void updateCreaturesFlock(Player &player);
	void drawCreatures(Player &p);
	void checkBulletPosition(float f_bullet_x, float f_bullet_y);
	void checkBulletPosition(ofxVec2f ofxvec_gun_pos, float f_gun_radius);
	
	//SETTERS
	void setNewCreatureArchetype(string s_creature_type, 
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
	
	//GETTERS
	
protected:
	
	int   mi_creature_max;
	bool  mb_repel;
	bool  mb_creatureHit;
	float mf_radius;
	float mf_repulsion_radius;
	float mf_strength;
	float mf_too_close_dist;
	float mf_in_sight_dist;
	float mf_spawn_x;
	float mf_spawn_y;
	float mf_dist;
	float mleft_x;
	float mleft_y;
	float mright_x; 
	float mright_y;  	 
	float mtop_x;      
	float mtop_y;    
	float mbottom_x;  
	float mbottom_y; 
	
	
	int offset;
	//Counter that determines when an enemy should attack player from closest position
	int nSurprise;
	// Stores copies of creatures to be used
	std::vector<Creature> creature_archetypes;
	
	// stores all the creatures drawn on play field
	std::vector<Creature> creatures;
	
};

#endif