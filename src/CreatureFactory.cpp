/*
 *  CreatureFactory.cpp
 *  openFrameworks
 *
 *  Created by Axel Esquite on 10/10/09.
 *  Copyright 2009 Axel Esquite. All rights reserved.
 *
 */

#include "CreatureFactory.h"

CreatureFactory::CreatureFactory() {
	// init members with default values
	mb_repel = false;
}

//------------------------------------------------------------
void CreatureFactory::startFactory(bool b_repel, float f_radius, float f_strength, float f_too_close_dist, float f_in_sight_distance) {
	
	mb_repel			= b_repel;
	mf_radius			= f_radius;
	mf_strength			= f_strength;
	mf_too_close_dist	= f_too_close_dist;
	mf_in_sight_dist	= f_in_sight_distance;

}

//------------------------------------------------------------
void CreatureFactory::updateCreatureMax(int i_creature_max) {
	
	// TODO: Amount to spawn should be
	// as time played > current time between spawns < last time between spawns 
	mi_creature_max = i_creature_max;
	spawnCreatures();
	
}

//------------------------------------------------------------
void CreatureFactory::findEdgeToSpawn(float f_player_x, float f_player_y) {
	
	float f_screen_x_diff = ofGetWidth() - f_player_x;
	float f_screen_y_diff = ofGetHeight() - f_player_y;
		
	if(f_screen_x_diff > f_player_x)
	{
		mf_spawn_x = ofGetWidth();
	}
	else if(f_screen_x_diff < f_player_x)
	{
		mf_spawn_x = 0.0f;
	}
	else
	{
		// you're dead center
		// TODO: randomize between zero and screen width
		
		mf_spawn_x = 0.0f;
	}
	
	if(f_screen_y_diff > f_player_y)
	{
		mf_spawn_y = ofGetHeight();
	}
	else if(f_screen_y_diff < f_player_y)
	{
		mf_spawn_y = 0.0f;
	}
	else
	{
		// you're dead center
		// TODO: randomize between zero and screen width
		
		mf_spawn_y = 0.0f;
	}
}

//------------------------------------------------------------
void CreatureFactory::spawnCreatures() {

	// adds however many creatures needed until creature_max is met
	while(creatures.size() < mi_creature_max)
	{
		// Create a creature
		Creature creature;
		
		// This is temporary just to make sure that creatures are spawning.
		// TODO: this should check GAME or somthing to see if it's ok to 
		// include and deploy particular creatures
		// creature_archetypes[ofRandom(0, creature_archetypes.size())];
		
		creature.setAllProperties(creature_archetypes[0]);
		creature.loadSprite();
		
		// set creature position
		creature.setPosition((mf_spawn_x * ofRandomf()), (mf_spawn_y * ofRandomf()));
		creatures.push_back(creature);
	}
	
}

//------------------------------------------------------------
void CreatureFactory::updateCreaturesFlock(Player &player) {

	// updates creature flock 
	for(int i=0; i<creatures.size(); i++)
	{
		creatures[i].resetForce();
		
		creatures[i].addAttractionForce(player.getPlayerX(), 
										player.getPlayerY(), 
										creatures[i].getRepulsionRadius(), 
										creatures[i].getStrength());
		
		
		for(int j=0; j<creatures.size(); j++)
		{
			
			if(j == i)
			{
				continue;
			}
			
			mf_dist = (creatures[i].getPosition() - creatures[i].getPosition()).length();
			
			if(mf_dist <= creatures[i].getInSight())
			{
				if(mf_dist <= creatures[i].getTooClose())
				{
					creatures[i].addRepulsionForce(creatures[j].getPosition(), 
												   creatures[j].getRepulsionRadius()/4, 
												   creatures[j].getScale());
				}
			}
		}
		
		creatures[i].addDampingForce();
		
		creatures[i].update();
		
		if(player.checkHit(creatures[i].getPosition(), creatures[i].getRadius()) == true)
		{
			// apply damage to player 
			player.deliverHit(creatures[i].getDamage());
		}
	}

}

//------------------------------------------------------------
void CreatureFactory::drawCreatures() {

	// draw our flock
	for(int i=0; i<creatures.size(); i++)
	{
		creatures[i].draw();
	}

}

//------------------------------------------------------------
void CreatureFactory::checkBulletPosition(float f_bullet_x, float f_bullet_y) {
	
	for(int i = 0; i<creatures.size(); i++)
	{
		// A creature knows when it is hit
		mb_creatureHit = creatures[i].checkHit(f_bullet_x, f_bullet_y);
		
		// kill creature if hit or show a bullet hole on ground if not hit
		if(mb_creatureHit == true)
		{
			//creatures[i].kill();
			creatures.erase(creatures.begin()+i);
			
			// update game score with all the info it needs to keep track of stats
			// TODO: add more params to update. it should keep track of info about the creature and player for stats
			//gameScore.update(2);
		}
		else
		{
			// TODO: display a bullet hole sprite 
			//printf("no hit\n");
		}
	
	
	}
}

//------------------------------------------------------------
void CreatureFactory::checkBulletPosition(ofxVec2f ofxvec_gun_pos, float f_gun_radius) {
	
	for(int i = 0; i<creatures.size(); i++)
	{
		// A creature knows when it is hit
		mb_creatureHit = creatures[i].checkHit(ofxvec_gun_pos, f_gun_radius);
		
		// kill creature if hit or show a bullet hole on ground if not hit
		if(mb_creatureHit == true)
		{
			//creatures[i].kill();
			creatures.erase(creatures.begin()+i);
			
			// update game score with all the info it needs to keep track of stats
			// TODO: add more params to update. it should keep track of info about the creature and player for stats
			//gameScore.update(2);
		}
		else
		{
			// TODO: display a bullet hole sprite 
			//printf("no hit\n");
		}
		
	}
}

//------------------------------------------------------------
void CreatureFactory::setNewCreatureArchetype(string s_creature_type, 
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
											  int i_frame_rate) {
		
	// take these properties and set them in a new creature
	Creature new_archetype;
	
	new_archetype.setAllProperties(s_creature_type,
								   f_damping, 
								   b_repel, 
								   f_radius, 
								   f_strength, 
								   f_too_close, 
								   f_in_sight,
								   f_scale,
								   f_repulsion_radius,
								   s_sprite,
								   s_sounds,
								   f_damage_rate,
								   f_hit_points,
								   i_frame_rate);
	//new_archetype.loadSounds();
	// pop the new instance of a creature into an archetype holder array
	creature_archetypes.push_back(new_archetype);
}