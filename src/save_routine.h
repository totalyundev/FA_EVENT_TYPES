#pragma once
#include <pebble.h>
#define PK_FIRST_LAUNCH 0
#define PK_COUNTER 1
#define PK_ZERO_TIME 2
#define PK_RADIO_BUTTON 3
#define PK_CHECKBOX 4
#define PK_LICKERT_SCALE 5
#define PK_COUNTER_STRUCT 6
#define PK_YES_NO 7
void initial_save();
void read_persist_data();
void save_persist_data();
void yes_no_save_data(bool ans, char menu_lvl);
void lickert_save_data(int8_t ans, char menu_lvl);
void checkbox_save_data(bool *ans, char menu_lvl);
void radio_button_save_data(int8_t answer, char menu_lvl);
//zero time
typedef struct zero_time {
	uint16_t year_day;
	int8_t hour;
	int8_t minute;
	
}__attribute__((__packed__)) zero_time;	


typedef struct question_counter {
	int8_t radio_button;
	int8_t checkbox;
	int8_t lickert_scale;
	int8_t yes_no;
	
}__attribute__((__packed__)) question_counter;	

//radio button
typedef struct radio_button_event {
	int16_t Timestamp; //minutes from start
	int8_t Answer_no; //that's easy
	int8_t Event_no; //for chain question purpose, unused in demo, har 0 val
	char Answer_group; //position in menu
	
}__attribute__((__packed__)) radio_button_event[50];	


//checkbox
typedef struct checkbox_event {
	int16_t Timestamp; //minutes from start
	int8_t Event_no;
	bool answer[5];
	
	//think about realization of this crap
	int8_t Answer_group; //position in menu
	
}__attribute__((__packed__)) checkbox_event[50];	

//LIckert scale
typedef struct lickert_scale_event {
	int16_t Timestamp; //minutes from start
	int8_t Event_no;
	int8_t Answer_group;
	int8_t Answer; //in case of multi question sets
	
}__attribute__((__packed__)) lickert_scale_event[50];	

//YES/NO
typedef struct yes_no_event {
	int16_t Timestamp; //minutes from start
	int8_t Event_no;
	bool Answer;
	int8_t Answer_group; //in case of multi question sets
	
}__attribute__((__packed__)) yes_no_event[60];	