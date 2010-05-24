/*
 *  GameUtils.cpp
 *  openFrameworks
 *
 *  Created by Axel Esquite on 5/1/10.
 *  Copyright 2010 Axel Esquite. All rights reserved.
 *
 */

#include "GameUtils.h"

//------------------------------------------------------------
string wrapString(string text, ofTrueTypeFont &ofttf_object, int width) {
	
	string typeWrapped = "";
	string tempString = "";
	vector <string> words = ofSplitString(text, " ");
	
	for(int i=0; i<words.size(); i++) {
		
		string wrd = words[i];
		//cout << wrd << endl;
		
		tempString += wrd + " ";
		int stringwidth = ofttf_object.stringWidth(tempString);
		if(stringwidth >= width) {
			tempString = "";
			typeWrapped += "\n";
		}
		
		typeWrapped += wrd + " ";
	}
	
	return typeWrapped;
	
}

//------------------------------------------------------------
/*bool checkHit(float f_x_pos, float f_y_pos, float f_gun_radius){
	
	ofxVec2f ofxvec_gun_pos;
	
	ofxvec_gun_pos.set(f_x_pos, f_y_pos);
	
	float f_hit_radius = mf_radius + f_gun_radius;
	
	ofxVec2f ofxvec_distance = ofv_pos - ofxvec_gun_pos;
	
	float f_current_length = ofxvec_distance.length();
	
	if(f_current_length < f_hit_radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}*/

//------------------------------------------------------------
bool checkHit(ofxVec2f ofxvec_gun_pos, float f_gun_radius, ofxVec2f ofv_pos, float f_radius){
		
	float f_hit_radius			= f_radius + f_gun_radius;
	
	ofxVec2f ofxvec_distance	= ofv_pos - ofxvec_gun_pos;

	//cout<<"ofv_pos: x-> "<<ofv_pos.x<<", y-> "<<ofv_pos.y<<", ofxvec_gun_pos: x-> "<<ofxvec_gun_pos.x<<", y-> "<<ofxvec_gun_pos.y<<endl;
	
	float f_current_length		= ofxvec_distance.length();
		
	//cout<<"f_hit_radius: "<<f_hit_radius<<", f_current_length: "<<f_current_length<<endl;
	
	if(f_current_length < f_hit_radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//------------------------------------------------------------
bool checkHit(ofxVec2f ofxvec_gun_pos, float f_gun_radius, float f_x_pos, float f_y_pos, float f_radius){
	
	float f_hit_radius			= f_radius + f_gun_radius;
	
	ofxVec2f ofv_pos;
	
	ofv_pos.set(f_x_pos,f_y_pos);
	
	ofxVec2f ofxvec_distance	= ofv_pos - ofxvec_gun_pos;
	
	//cout<<"ofv_pos: x-> "<<ofv_pos.x<<", y-> "<<ofv_pos.y<<", ofxvec_gun_pos: x-> "<<ofxvec_gun_pos.x<<", y-> "<<ofxvec_gun_pos.y<<endl;
	
	float f_current_length		= ofxvec_distance.length();
	
	//cout<<"f_hit_radius: "<<f_hit_radius<<", f_current_length: "<<f_current_length<<endl;
	
	if(f_current_length < f_hit_radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}