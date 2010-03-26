/*
 *  GameScore.h
 *  openFrameworks
 *
 *  Created by Axel Esquite on 11/11/09.
 *  Copyright 2009 Axel Esquite. All rights reserved.
 *
 */

#ifndef _GAME_SCORE
#define _GAMES_SCORE

#include "ofMain.h"

class GameScore {
	
public:
	GameScore();	
	
	// METHODS
	void update(int i_amount);
	void outputAsString(int i_x_position, int i_y_position);
	string stringifyScore(int i_score_to_stringify);
	
	// SETTERS
	void setFormat(string s_score_type, int i_start_position, string s_score_font, int i_score_font_size);
	
	// GETTERS
	
protected:
	
	string s_type_of_score;
	
	// keeps score - FOR BOTH TIME OR INT
	int i_gameScore;
	
	// stringified score
	string s_gameScoreText;
	
	// TODO: Anything related to this will be in a scoring class and used in game manager class
	ofTrueTypeFont ofttf_gameScoreText;
	
};

#endif