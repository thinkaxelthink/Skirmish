/*
 *  GameScore.cpp
 *  openFrameworks
 *
 *  Created by Axel Esquite on 11/11/09.
 *  Copyright 2009 Axel Esquite. All rights reserved.
 *
 */

#include "GameScore.h"

GameScore::GameScore() {
	
}

void GameScore::outputAsString(int i_x_position, int i_y_position){
	
	ofSetColor(0,0,0);
	ofFill();	
	//730, 40
	ofttf_gameScoreText.drawString(s_gameScoreText, i_x_position, i_y_position);
	
}

// always hand off in seconds if time
// if Number, int
void GameScore::update(int i_amount){
	
	if(s_type_of_score == "TIME")
	{
		
		int i_minutes = i_amount/60;
		int i_seconds = i_amount - (i_minutes * 60);
		
		string s_temp_minutes = stringifyScore(i_minutes);
		string s_temp_seconds;
		
		((i_seconds < 10) ? s_temp_seconds = "0" : s_temp_seconds = "");
		
		s_temp_seconds.append(stringifyScore(i_seconds));
		
		s_gameScoreText = s_temp_minutes;
		
		s_gameScoreText.append(":");
		
		s_gameScoreText.append(s_temp_seconds);
	}
	else if(s_type_of_score == "NUMBER")
	{
		i_gameScore += i_amount;
		
		s_gameScoreText = stringifyScore(i_gameScore);
			
	}
	
}

// SETTERS

void GameScore::setFormat(string s_score_type, int i_start_position, string s_score_font, int i_score_font_size){
	
	s_type_of_score = s_score_type;
	
	i_gameScore	= i_start_position;
	
	s_gameScoreText = stringifyScore(i_gameScore);
	
	if(s_type_of_score == "TIME")
	{
		//TODO: this will have to also accept a start point for a count down time if defined
		// assumes time counts up
		s_gameScoreText = "0:00";
	}
	
	// default is DIN, size 30
	ofttf_gameScoreText.loadFont(s_score_font, i_score_font_size);

}


string GameScore::stringifyScore(int i_score_to_stringify){
	
	string s_stringifiedScore;
	
	ostringstream scoreToStringify;
	scoreToStringify << i_score_to_stringify;
	s_stringifiedScore = scoreToStringify.str();
	
	return s_stringifiedScore;
}

