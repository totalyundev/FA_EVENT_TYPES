#include <pebble.h>
#include "first_launch.h"
#include "main.h"
#include "save_routine.h"
// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_roboto_condensed_21;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_3;

static int s_uptime=0;
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	s_uptime++;
 if(s_uptime==1) {
	 
	 text_layer_set_text(s_textlayer_1, "OK. Restart.");
	   text_layer_set_text(s_textlayer_2, ".");
	  text_layer_set_text(s_textlayer_3, ".");

 }
	//	hide_stopwatch();
	if (s_uptime==3){
		//window_stack_pop_all(true);
		//disp main menu
		//show_first_menu();
		window_stack_pop_all( true);
		// tick_timer_service_unsubscribe();
	}
	
}

static void start_data_collect(void){
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
	
	
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	   APP_LOG(APP_LOG_LEVEL_INFO, "Rozpocznij badanie");
persist_write_bool(PK_FIRST_LAUNCH, true);
		//window_stack_pop_all(true);
initial_save();
	start_data_collect();

	//never ever show this window, start saving data
	
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "EXIT");
	//quit app
	window_stack_pop_all(true);
	
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Test aplikacji");
	//show UI without data save
	hide_first_launch();
}


static void click_config_provider(void *context) {
	
	window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
	window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void initialise_ui(void) {
  s_window = window_create();
 
  
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(2, 62, 137, 51));
  text_layer_set_text(s_textlayer_1, "Rozpocznij badanie");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_1, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(2, 128, 136, 29));
  text_layer_set_text(s_textlayer_2, "Test aplikacji");
  text_layer_set_text_alignment(s_textlayer_2, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_2, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
  
  // s_textlayer_3
  s_textlayer_3 = text_layer_create(GRect(1, 25, 39, 25));
  text_layer_set_text(s_textlayer_3, "EXIT");
  text_layer_set_font(s_textlayer_3, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_2);
  text_layer_destroy(s_textlayer_3);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_first_launch(void) {
  initialise_ui();
	persist_write_bool(PK_FIRST_LAUNCH, false);

  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
  window_set_click_config_provider(s_window, (ClickConfigProvider)click_config_provider); //buttons useful now

}

void hide_first_launch(void) {
  window_stack_remove(s_window, true);
}
