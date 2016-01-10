
#include "main.h"
#include "radio_button_window.h"
#include "dialog_message_window.h"
#define ID_STARTING 0
static Window *s_main_window;
static MenuLayer *s_menu_layer;
static TextLayer *s_list_message_layer;
static Layer *primitives_layer;

static int s_current_selection = 0;

static char* subtitles[] = {
	"first one",
	"second",
	"third",
	"fourth",
	"SUBMIT"
		
};

static void canvas_update_proc(Layer *this_layer, GContext *ctx) {
    
    graphics_draw_rect(ctx, GRect(1, 1, 142, 28));
}

static void update_text(int index) {
	static char s_puff[24];
		snprintf(s_puff, sizeof(s_puff), subtitles[index]);
	text_layer_set_text(s_list_message_layer, s_puff);
	
} 

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
	//do nothing
	return;
}

static void click_config_provider(void *context) {

		window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

static void get_selected_index(MenuLayer *menu_layer, MenuIndex *new_index, MenuIndex old_index, void *context){
	update_text(new_index->row);
	APP_LOG(APP_LOG_LEVEL_INFO, "CHUJ W DUPIE");

} 

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return RADIO_BUTTON_WINDOW_NUM_ROWS + 1;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
  if(cell_index->row == RADIO_BUTTON_WINDOW_NUM_ROWS) {
    // This is the submit item
    menu_cell_basic_draw(ctx, cell_layer, "Submit", NULL, NULL);
  } else {
    // This is a choice item
    static char s_buff[16];
    snprintf(s_buff, sizeof(s_buff), "Choice %d", (int)cell_index->row);
    menu_cell_basic_draw(ctx, cell_layer, s_buff, NULL, NULL);

    GRect bounds = layer_get_bounds(cell_layer);
    GPoint p = GPoint(bounds.size.w - (3 * RADIO_BUTTON_WINDOW_RADIO_RADIUS), (bounds.size.h / 2));

    // Selected?
    if(menu_cell_layer_is_highlighted(cell_layer)) {
      graphics_context_set_stroke_color(ctx, GColorWhite);
      graphics_context_set_fill_color(ctx, GColorWhite);
    } else {
      graphics_context_set_fill_color(ctx, GColorBlack);
    }

    // Draw radio filled/empty
    graphics_draw_circle(ctx, p, RADIO_BUTTON_WINDOW_RADIO_RADIUS);
    if(cell_index->row == s_current_selection) {
      // This is the selection
      graphics_fill_circle(ctx, p, RADIO_BUTTON_WINDOW_RADIO_RADIUS - 3);
    }
  }
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return PBL_IF_ROUND_ELSE(
    menu_layer_is_index_selected(menu_layer, cell_index) ? 
      MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT : MENU_CELL_ROUND_UNFOCUSED_TALL_CELL_HEIGHT,
    28);
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  if(cell_index->row == RADIO_BUTTON_WINDOW_NUM_ROWS) {
    // Do something with user choice
    APP_LOG(APP_LOG_LEVEL_INFO, "Submitted choice %d", s_current_selection);
  
		scenario(0,'q');
		// window_stack_pop(true);
	//dialog_message_window_push();
  } else {
    // Change selection
    s_current_selection = cell_index->row;
    menu_layer_reload_data(menu_layer);
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

	//txt layer for subtitle box
	s_list_message_layer = text_layer_create(GRect(5,0,140,28));
	text_layer_set_font(s_list_message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_list_message_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(s_list_message_layer));
	update_text(ID_STARTING);
	
	
  s_menu_layer = menu_layer_create(GRect(0, 30, 144, 150));
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = get_num_rows_callback,
      .draw_row = draw_row_callback,
      .get_cell_height = get_cell_height_callback,
      .select_click = select_callback,
			.selection_will_change = get_selected_index,

  });
 	menu_layer_set_selected_index(s_menu_layer, MenuIndex(0,ID_STARTING), MenuRowAlignNone, true);

  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));

		//to display frame
    primitives_layer = layer_create(bounds);
    layer_set_update_proc(primitives_layer, canvas_update_proc);
    layer_add_child(window_layer, primitives_layer);

}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);

  window_destroy(window);
  s_main_window = NULL;
}

void radio_button_window_push() {
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
