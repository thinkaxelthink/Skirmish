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
	mb_repel   = false;
	mleft_x   =  0;
	mleft_y   =	 ofGetHeight()/2;
	mright_x  =  ofGetWidth();
	mright_y  =	 ofGetHeight()/2;
	mtop_x    =  ofGetWidth()/2;
	mtop_y    =  0;
	mbottom_x =  ofGetWidth()/2;
	mbottom_y =  ofGetHeight();
	offset    =  20;
	nSurprise =  ofRandom(4,7);
	//mi_creature_max = 0;
	
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
void CreatureFactory::updateCreatureMax(int i_creature_max, float f_player_x, float f_player_y ) {
	
	// TODO: Amount to spawn should be
	// as time played > current time between spawns < last time between spawns 

		mi_creature_max = i_creature_max;
		findEdgeToSpawn(f_player_x, f_player_y);
		spawnCreatures();
	//}
//	else {
//		return;

	
}

//------------------------------------------------------------
void CreatureFactory::findEdgeToSpawn(float f_player_x, float f_player_y) {
	
	//Calculates player's distance from midpoints along each edge
	//Then spawns enemy from farthest edge, with the exception of
	//Suprise attacks, which are determined by a counter with a randomly generated duration.
	
	// used to specifiy the closest or farthest wall to spawn from. 0 is closest, 3 is farthest. 
	int spawn_side = 3;
	
	//Check if the suprise counter is at 0. If so, spawn from wall closest to player and reset it.
	if (nSurprise == 0)
	{
		spawn_side = 0;
		nSurprise = ofRandom(4, 7);
	}

	// distance checking
	float dx_l = f_player_x - mleft_x;
	float dy_l = f_player_y - mleft_y; 
	
	float dx_r = f_player_x - mright_x; 
	float dy_r = f_player_y - mright_y;
	
	float dx_b = f_player_x - mbottom_x;
	float dy_b = f_player_y - mbottom_y;
	
	float dx_t = f_player_x - mtop_x;
	float dy_t = f_player_y - mtop_y;
	
	float dist_l = sqrt( (dx_l*dx_l) + (dy_l*dy_l));
	float dist_r = sqrt( (dx_r*dx_r) + (dy_r*dy_r));
	float dist_t = sqrt( (dx_t*dx_t) + (dy_t*dy_t));
	float dist_b = sqrt( (dx_b*dx_b) + (dy_b*dy_b)); 
	
	//create arrays to store distances for sorting and comparison
	float adist[4];
	float sdist[4];
	
	//Store distances in unsort and sort arrays
	adist[0] = dist_l;
	adist[1] = dist_r;
	adist[2] = dist_t;
	adist[3] = dist_b;
	
	sdist[0] = adist[0];
	sdist[1] = adist[1];
	sdist[2] = adist[2];
	sdist[3] = adist[3];
	
	//Sort distances in array
	sort(sdist, sdist + 4);

	//Test the unsorted array against sorted to get the Spawn Point
	
	//Left Spawn
	if(sdist[spawn_side] == adist[0])
	{
		mf_spawn_x = -offset;
		mf_spawn_y = ofRandom(0, ofGetHeight());
	}
	//Right Spawn
	else if(sdist[spawn_side] == adist[1])
	{
		mf_spawn_x = ofGetWidth() + offset;
		mf_spawn_y = ofRandom(0, ofGetHeight());
	}
	//Top Spawn
	else if(sdist[spawn_side] == adist[2])
	{
		mf_spawn_x = ofRandom(0, ofGetWidth());
		mf_spawn_y = -offset;
	}
	//Bottom Spawn
	else if(sdist[spawn_side] == adist[3])
	{
		mf_spawn_x = ofRandom(0, ofGetWidth());
		mf_spawn_y = ofGetHeight() + offset;
	}
	nSurprise--;
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
		creature.setPosition((mf_spawn_x ), (mf_spawn_y ));
		creatures.push_back(creature);
	}
	
}

//------------------------------------------------------------
void CreatureFactory::resetCreatures() {
	creatures.clear();
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
			player.isHit = true;
		//	creatures[i].die();
		//	creatures.erase(creatures.begin() + i);
		//	cout << "I KILLED THIS FUCKER : " << i << endl;
		}
		else
		{
			player.isHit = false;
		}
	}

}

//------------------------------------------------------------
void CreatureFactory::drawCreatures(Player &p) {

	// draw our flock
	if(creatures.size() > 0)
	{
		for(int i=0; i<creatures.size(); i++)
		{
			creatures[i].draw();
		}
	}
}

//------------------------------------------------------------
void CreatureFactory::checkBulletPosition(float f_bullet_x, float f_bullet_y) {
	if(creatures.size() > 0)
	{
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
}

//------------------------------------------------------------
void CreatureFactory::checkBulletPosition(ofxVec2f ofxvec_gun_pos, float f_gun_radius) {
	if(creatures.size() > 0)
	{
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
