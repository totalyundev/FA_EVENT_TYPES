/*
gr8 ideas:
-multiclick+longclick as a method to get to hidden option secured by PIN, which wipe out the data


*/

#include <pebble.h>
#include "radio_button_window.h"
#include "Lickert_scale_window.h"
#include "dialog_message_window.h"
#include "checkbox_window.h"
#include "main.h"
#include "yes_no_window.h"
#include "save_routine.h"
#include "first_launch.h"

#define MENU_CELL_HEIGHT 28

#define NUM_WINDOWS 6
bool first_run = false;
bool first_chain = false;
int8_t first_chain_counter = 0;

static Window *s_main_window;
static MenuLayer *s_menu_layer;
static GBitmap *save_icon;
//this is the func to handle all of the paths
void scenario(int index, char choice) {

	if(first_chain){
		switch(first_chain_counter){
						
            case 1:
            radio_button_window_push();
						first_chain_counter++;
            break;
            case 2:
            checkbox_window_push();
      			first_chain_counter++;      
						break;
            case 3:
            likert_scale_window_push();
      			first_chain_counter++;      
						break;
            case 4:
						dialog_message_window_push();
						break;
           
			
		}
		        
		
		
	} 
	else {
		switch (choice) {
        //main menu
        case 'a':
        switch (index) {
            case 0:
            dialog_message_window_push();
            break;
            case 1:
            radio_button_window_push();
            break;
            case 2:
            checkbox_window_push();
            break;
            case 3:
            likert_scale_window_push();
            break;
            case 4:
						show_yes_no_window();
						break;
            case 5:
            first_chain = true;
											first_chain_counter++;

						show_yes_no_window();
            break;        
        }
			break;
			case'q':
			default:
			 dialog_message_window_push();
			break;
        return;
    }
  
		
	}
}

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return NUM_WINDOWS;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
      menu_cell_basic_draw(ctx, cell_layer, "Save", NULL, save_icon);
      break;
    case 1:
      menu_cell_basic_draw(ctx, cell_layer, "Radio button", NULL, NULL);
      break;
    case 2:
      menu_cell_basic_draw(ctx, cell_layer, "Checkbox", NULL, NULL);
      break;
    case 3:
      menu_cell_basic_draw(ctx, cell_layer, "Likert", NULL, NULL);
      break;
    case 4:
      menu_cell_basic_draw(ctx, cell_layer, "Yes/No", NULL, NULL);
      break;
    case 5:
      menu_cell_basic_draw(ctx, cell_layer, "Chain example", NULL, NULL);
      break;
   
    default:
      break;
  }
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return PBL_IF_ROUND_ELSE(
    menu_layer_is_index_selected(menu_layer, cell_index) ?
      MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT : MENU_CELL_ROUND_UNFOCUSED_TALL_CELL_HEIGHT,
    MENU_CELL_HEIGHT);
}



static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
	static char a = 'a';
  switch(cell_index->row) {
   /*
		case 0:
 dialog_message_window_push();
      break;
    case 1:
 radio_button_window_push();
      break;
    case 2:
 checkbox_window_push();
      break;
    case 3:
 likert_scale_window_push();
      break;
    case 4:
  //    radio_button_window_push();
      break;
    case 5:
		//
      break;
 */
    default:
		scenario(cell_index->row, a);
      break;
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  //GRect bounds = layer_get_bounds(window_layer);
save_icon = gbitmap_create_with_resource(RESOURCE_ID_HEHE);
  s_menu_layer = menu_layer_create(GRect(0, 0, 144, 200));
  menu_layer_set_click_config_onto_window(s_menu_layer, window);

  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = get_num_rows_callback,
      .draw_row = draw_row_callback,
      .get_cell_height = get_cell_height_callback,
      .select_click = select_callback,
  });
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
}

static void init() {
	bool save_data;
		save_data=persist_read_bool(PK_FIRST_LAUNCH);
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
  });
  window_stack_push(s_main_window, true);
	if(save_data==false){
		
		show_first_launch();	
	}
	
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}