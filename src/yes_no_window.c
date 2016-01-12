#include <pebble.h>
#include "yes_no_window.h"
#include "main.h"
#include "save_routine.h"
static Window *s_main_window;
static TextLayer *s_label_layer;
static ActionBarLayer *s_action_bar_layer;
static char menu_lvl;
static GBitmap *s_tick_bitmap, *s_cross_bitmap;

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	yes_no_save_data(true, menu_lvl);
	scenario(0,'q');
	
	//dialog_message_window_push();
APP_LOG(APP_LOG_LEVEL_INFO, "up");
}
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  
		yes_no_save_data(false, menu_lvl);
	scenario(0,'q');
	//dialog_message_window_push();
	APP_LOG(APP_LOG_LEVEL_INFO, "missing chromosome");
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
	//do nothing
	return;
}

static void click_config_provider(void *context) {
		//window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
		window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
		window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
		window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);

}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);


  const GEdgeInsets label_insets = {.top = 112, .right = ACTION_BAR_WIDTH, .left = ACTION_BAR_WIDTH / 2};
  s_label_layer = text_layer_create(GRect(5,5,100,150));
  text_layer_set_text(s_label_layer, "THIS IS PLACE FOR QUESTION");
  text_layer_set_background_color(s_label_layer, GColorClear);
  text_layer_set_text_alignment(s_label_layer, GTextAlignmentCenter);
  text_layer_set_font(s_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_label_layer));

  s_tick_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TICK_ICON);
  s_cross_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CROSS);

  s_action_bar_layer = action_bar_layer_create();
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, s_tick_bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_cross_bitmap);
  action_bar_layer_add_to_window(s_action_bar_layer, window);
	  action_bar_layer_set_click_config_provider(s_action_bar_layer, click_config_provider);
}

static void window_unload(Window *window) {
  text_layer_destroy(s_label_layer);
  action_bar_layer_destroy(s_action_bar_layer);
  gbitmap_destroy(s_tick_bitmap);
  gbitmap_destroy(s_cross_bitmap);

  window_destroy(window);
  s_main_window = NULL;
}

void show_yes_no_window(char choice){
	menu_lvl=choice;
	if(!s_main_window) {
    s_main_window = window_create();
    window_set_background_color(s_main_window, GColorWhite);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
	window_set_click_config_provider(s_main_window, (ClickConfigProvider)click_config_provider);
  window_stack_push(s_main_window, true);
	
}