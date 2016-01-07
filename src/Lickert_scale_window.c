#include <pebble.h>
#include "Lickert_scale_window.h"
#include "dialog_message_window.h"
#include "main.h"

static Window *s_main_window;
static MenuLayer *s_menu_layer;
static TextLayer *s_list_message_layer, *s_question_layer;
static Layer *primitives_layer;

static int highlight_element=2;

static char* s_title[] = {
	"Zdecydowanie nie",
	"Raczej nie",
	"Ani tak, ani nie",
	"Raczej tak",
	"Zdecydowanie tak"
		
};

static void update_text(int index) {
	static char s_puff[24];
		snprintf(s_puff, sizeof(s_puff), s_title[index]);
	text_layer_set_text(s_list_message_layer, s_puff);
	
} 

static void canvas_update_proc(Layer *this_layer, GContext *ctx) {
//  GRect bounds = layer_get_bounds(this_layer);
	graphics_draw_line(ctx, GPoint(0,128), GPoint(144,128));
	graphics_draw_line(ctx, GPoint(125,0), GPoint(125,128));
}
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	   scenario(0,'q');		
	//dialog_message_window_push();
	return;
}
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	menu_layer_set_selected_next(s_menu_layer, true, MenuRowAlignNone, true);
	if(highlight_element>=1){
	highlight_element--;
	update_text(highlight_element);	
	}
	else return;
	
}
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	menu_layer_set_selected_next(s_menu_layer, false, MenuRowAlignNone, true);
if(highlight_element<=3){
		highlight_element++;
		update_text(highlight_element);
			}
	else return;

	
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
	//do nothing
	return;
}

static void click_config_provider(void *context) {
		window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
		window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
		window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
		window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);

}

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return LIST_MESSAGE_WINDOW_NUM_ROWS;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
  static char s_buff[16];
  snprintf(s_buff, sizeof(s_buff), "%d", (int)cell_index->row+1);
  menu_cell_basic_draw(ctx, cell_layer, s_buff, NULL, NULL);
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return PBL_IF_ROUND_ELSE(
    menu_layer_is_index_selected(menu_layer, cell_index) ?
      MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT : MENU_CELL_ROUND_UNFOCUSED_TALL_CELL_HEIGHT,
    LIST_MESSAGE_WINDOW_CELL_HEIGHT);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
	const GEdgeInsets message_insets = {.top = 140};
  
	s_list_message_layer = text_layer_create(GRect(5,140,160,50));
  text_layer_set_text_alignment(s_list_message_layer, GTextAlignmentLeft);
 update_text(2);
//	text_layer_set_text(s_list_message_layer, "1 2 3 4 5 6 7 8 9 10");
  layer_add_child(window_layer, text_layer_get_layer(s_list_message_layer));
	
  s_menu_layer = menu_layer_create(GRect(125, 2, 40, LIST_MESSAGE_WINDOW_MENU_HEIGHT));
 // menu_layer_set_click_config_onto_window(s_menu_layer, window);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = get_num_rows_callback,
      .draw_row = draw_row_callback,
      .get_cell_height = get_cell_height_callback,
  });
	menu_layer_set_selected_index(s_menu_layer, MenuIndex(0,2), MenuRowAlignNone, true);
 	layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));

	//question layer
	s_question_layer = text_layer_create(GRect(5,5,122,130));
	text_layer_set_text_alignment(s_question_layer, GTextAlignmentLeft);
	text_layer_set_text(s_question_layer, "This is place for question");
  layer_add_child(window_layer, text_layer_get_layer(s_question_layer));
	
  
	primitives_layer = layer_create(bounds);
	layer_add_child(window_layer, primitives_layer);
	layer_set_update_proc(primitives_layer, canvas_update_proc);
}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
  text_layer_destroy(s_list_message_layer);

  window_destroy(window);
  s_main_window = NULL;
}

void likert_scale_window_push() {
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
	window_set_click_config_provider(s_main_window, (ClickConfigProvider)click_config_provider);
  window_stack_push(s_main_window, true);
}