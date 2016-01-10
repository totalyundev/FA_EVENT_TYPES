#pragma once
#include <pebble.h>
#define PK_FIRST_LAUNCH 0
#define PK_ZERO_TIME x
#define PK_RADIO_BUTTON xx
#define PK_CHECKBOX xxx
#define PK_LICKERT_SCALE xxxx


#define PK_COUNTER XXX


//zero time
typedef struct zero_time {
	uint16_t year_day;
	int8_t hour;
	int8_t minute;
	
}__attribute__((__packed__)) zero_time;	

//radio button
typedef struct radio_button_event {
	int16_t Timestamp; //minutes from start
	int8_t Event_no; //for chain question purpose
	int8_t Answer_no;
	int8_t Answer_group; //in case of multi question sets
	
}__attribute__((__packed__)) radio_button_event[1];	

//checkbox
typedef struct checkbox_event {
	int16_t Timestamp; //minutes from start
	int8_t Event_no;
	bool answer[5];//think about realization of this crap
	int8_t Answer_group; //in case of multi question sets
	
}__attribute__((__packed__)) checkbox_event[1];	

//LIckert scale
typedef struct lickert_scale_event {
	int16_t Timestamp; //minutes from start
	int8_t Event_no;
	int8_t Answer_no;
	int8_t Answer_; //in case of multi question sets
	
}__attribute__((__packed__)) lickert_scale_event[1];	

//YES/NO
typedef struct yes_no_event {
	int16_t Timestamp; //minutes from start
	int8_t Event_no;
	bool answer;
	int8_t Answer_group; //in case of multi question sets
	
}__attribute__((__packed__)) yes_no_event[1];	