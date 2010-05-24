/*
 *  utils.h
 *  openFrameworks
 *
 *  Created by Axel Esquite on 5/1/10.
 *  Copyright 2010 Axel Esquite. All rights reserved.
 *
 */

#ifndef _GAME_UTILS
#define _GAME_UTILS

#include "ofMain.h"
#include "ofxVectorMath.h"

string wrapString(string text, ofTrueTypeFont &ofttf_object, int width);

//bool checkHit(float f_x_pos, float f_y_pos, float f_gun_radius);
bool checkHit(ofxVec2f ofxvec_gun_pos , float f_gun_radius, ofxVec2f ofv_pos, float f_radius);
bool checkHit(ofxVec2f ofxvec_gun_pos , float f_gun_radius, float f_x_pos, float f_y_pos, float f_radius);


#endif