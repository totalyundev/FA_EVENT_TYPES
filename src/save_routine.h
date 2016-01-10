#pragma once

#define PK_ZERO_TIME 0
#define PK_RADIO_BUTTON 1
#define PK_CHECKBOX 2
#define PK_LICKERT_SCALE 3


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