#include <pebble.h>
#include "save_routine.h"
//read size of -- get the right number dumbass
zero_time start_time;
zero_time get_time;
radio_button_event radio_button_data;
checkbox_event checkbox_data;
lickert_scale_event lickert_scale_data;
yes_no_event yes_no_data;
question_counter counter;
static void get_time_func(void);
static void zero_time_save(void);
static int8_t persist_counter=0;
static int8_t persist_counter_temp=0;

void initial_save(void){
	
	
		counter.yes_no=0;
		counter.radio_button=0;
		counter.lickert_scale=0;
		counter.checkbox=0;
		persist_write_data(PK_COUNTER_STRUCT, &counter, sizeof(counter));
		
	
		get_time_func();
		zero_time_save();
		
	
	
}

static int16_t timestamp_func(void){
		get_time_func();
		int16_t timestamp =0;
		int16_t year_day=get_time.year_day-start_time.year_day;
		int16_t hour=get_time.hour-start_time.hour;
		int16_t minute=get_time.minute-start_time.minute;
	
	timestamp = year_day*1440+hour*60+minute;
	return timestamp;
} 

static void get_time_func(void) {
	//func which get actual day, hour, minute to temp struct "get_time"
	//get time struct
  time_t temp =time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char j_buffer[8]; //day of the year 001-366
  static char H_buffer[8]; // hour (24)
  static char M_buffer[8]; // minutes
	
  strftime(j_buffer, sizeof(j_buffer),"%j",tick_time);
  strftime(H_buffer, sizeof(H_buffer),"%H",tick_time);
  strftime(M_buffer, sizeof(M_buffer),"%M",tick_time);
  
 	int16_t temp_j = atoi(j_buffer);
 	int8_t temp_H = atoi(H_buffer);
  int8_t temp_M = atoi(M_buffer);
	
	get_time.year_day=temp_j;
	get_time.hour=temp_H;
	get_time.minute=temp_M;
}

void yes_no_save_data(bool ans, char menu_lvl) {
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", ans?"true":"false");
	yes_no_data[counter.yes_no].Timestamp=timestamp_func();
	yes_no_data[counter.yes_no].Answer=ans;
	yes_no_data[counter.yes_no].Event_no=77;
	yes_no_data[counter.yes_no].Answer_group=menu_lvl;
	counter.yes_no++;
}

void lickert_save_data(int8_t ans, char menu_lvl){
	lickert_scale_data[counter.lickert_scale].Timestamp=timestamp_func();
	lickert_scale_data[counter.lickert_scale].Answer=ans;
	lickert_scale_data[counter.lickert_scale].Event_no=0;
	lickert_scale_data[counter.lickert_scale].Answer_group=menu_lvl;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "answer: %d", ans);
	counter.lickert_scale++;
}

void checkbox_save_data(bool *ans, char menu_lvl){
	
	checkbox_data[counter.checkbox].Timestamp=timestamp_func();
	for(int i=0;i<5;i++){
			checkbox_data[counter.checkbox].answer[i]=ans[i];

	}
	checkbox_data[counter.checkbox].Event_no=0;
	checkbox_data[counter.checkbox].Answer_group=menu_lvl;
	
	counter.checkbox++;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "answers");
for(int i=0;i<5;i++) {
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", ans[i]?"true":"false");
	}	

}


void radio_button_save_data(int8_t answer, char menu_lvl) {
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "answer: %d", answer);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "menu_lvl: %c", menu_lvl);
	
	radio_button_data[counter.radio_button].Timestamp=timestamp_func();
	radio_button_data[counter.radio_button].Answer_no=answer;
	radio_button_data[counter.radio_button].Event_no=0;
	radio_button_data[counter.radio_button].Answer_group=menu_lvl;
	counter.radio_button++;

	
}

static void zero_time_save(void){
	//used once
	start_time.year_day=get_time.year_day;
	start_time.hour=get_time.hour;
	start_time.minute=get_time.minute;
	persist_write_data(PK_ZERO_TIME, &start_time, sizeof(start_time));
		APP_LOG(APP_LOG_LEVEL_DEBUG, "base time NOT exists, SAVED");
}

void save_persist_data() {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "data saving");
	

	persist_write_data(PK_COUNTER_STRUCT, &counter, sizeof(counter));

	persist_write_data(PK_RADIO_BUTTON, &radio_button_data,sizeof(radio_button_data));
	persist_write_data(PK_CHECKBOX, &checkbox_data, sizeof(checkbox_data));
	persist_write_data(PK_YES_NO, &yes_no_data, sizeof(yes_no_data));
	persist_write_data(PK_LICKERT_SCALE, &lickert_scale_data, sizeof(lickert_scale_data));
	APP_LOG(APP_LOG_LEVEL_DEBUG, "data saved");
}
 



void read_persist_data() {
	if(persist_exists(PK_COUNTER)) {
		persist_counter = persist_read_int(PK_COUNTER);
		persist_counter_temp = persist_counter; 

		 APP_LOG(APP_LOG_LEVEL_DEBUG, "persist_counter_val: %d", persist_counter);
	}
	
	if(persist_exists(PK_ZERO_TIME)) {
		persist_read_data(PK_ZERO_TIME, &start_time, sizeof(start_time));
		APP_LOG(APP_LOG_LEVEL_DEBUG, "base time already exists");
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Base time:day_of_the_year: %d,hour: %d,minute: %d", start_time.year_day, start_time.hour, start_time.minute);
		
	}
	/*
	else {
		get_time_func();
		zero_time_save();
	}
	*/
	if(persist_exists(PK_COUNTER_STRUCT)){
		persist_read_data(PK_COUNTER_STRUCT, &counter, sizeof(counter));
		APP_LOG(APP_LOG_LEVEL_DEBUG, "radio counter:%d",counter.radio_button);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "checkbox counter:%d",counter.checkbox);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "yes_no counter:%d",counter.yes_no);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "lickert counter:%d",counter.lickert_scale);
		}
	
	
		
		
	if(persist_exists(PK_RADIO_BUTTON)) {
		persist_read_data(PK_RADIO_BUTTON, &radio_button_data, sizeof(radio_button_data));
		APP_LOG(APP_LOG_LEVEL_DEBUG, "//////////////////////////////RADIO BUTTON LOGS");					
		APP_LOG(APP_LOG_LEVEL_DEBUG, "i,timestamp,ans,ans_gr");
		for(int i=0;i<=counter.radio_button;i++){
					APP_LOG(APP_LOG_LEVEL_DEBUG, "%d,%d,%d,%c",i,	radio_button_data[i].Timestamp, radio_button_data[i].Answer_no, radio_button_data[i].Answer_group);

			
		}
	}
	if(persist_exists(PK_CHECKBOX)) {
				persist_read_data(PK_CHECKBOX, &checkbox_data, sizeof(checkbox_data));
				APP_LOG(APP_LOG_LEVEL_DEBUG, "//////////////////////////////CHECKBOX LOGS");					
							APP_LOG(APP_LOG_LEVEL_DEBUG, "i,timestamp,tyfytyfytyfytfy,ans_gr");
		for(int i=0;i<=counter.checkbox;i++){
					APP_LOG(APP_LOG_LEVEL_DEBUG, "%d,%d,%s,%s,%s,%s,%s,%c",i,	checkbox_data[i].Timestamp,checkbox_data[i].answer[0]?"1":"0",
									checkbox_data[i].answer[1]?"1":"0",checkbox_data[i].answer[2]?"1":"0",checkbox_data[i].answer[3]?"1":"0",checkbox_data[i].answer[4]?"1":"0", checkbox_data[i].Answer_group);

	}
	}
	if(persist_exists(PK_LICKERT_SCALE)) {
				persist_read_data(PK_LICKERT_SCALE, &lickert_scale_data, sizeof(lickert_scale_data));
				APP_LOG(APP_LOG_LEVEL_DEBUG, "//////////////////////////////LICKERT SCALE LOGS");					
		APP_LOG(APP_LOG_LEVEL_DEBUG, "i,timestamp,ans,ans_gr");					
		for(int i=0;i<=counter.lickert_scale;i++) {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "%d,%d,%d,%c",i,	lickert_scale_data[i].Timestamp, lickert_scale_data[i].Answer, lickert_scale_data[i].Answer_group);
			
			
		}

	}
	

	if(persist_exists(PK_YES_NO)) {
		persist_read_data(PK_YES_NO, &yes_no_data, sizeof(yes_no_data));
				APP_LOG(APP_LOG_LEVEL_DEBUG, "//////////////////////////////YES / NO LOGS");					

		APP_LOG(APP_LOG_LEVEL_DEBUG, "i,timestamp,ans,ans_gr");
		for(int i=0;i<=counter.yes_no;i++) {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "%d,%d,%s,%c", i,yes_no_data[i].Timestamp,yes_no_data[i].Answer?"T":"F", yes_no_data[i].Answer_group);
			
		}
		
	}
	
	
}