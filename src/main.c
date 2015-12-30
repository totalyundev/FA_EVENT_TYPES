#include <pebble.h>
#include "radio_button_window.h"
#define CHECKBOX_WINDOW_NUM_ROWS    4
#define CHECKBOX_WINDOW_BOX_SIZE    12
#define CHECKBOX_WINDOW_CELL_HEIGHT 28

#define NUM_WINDOWS 6

static Window *s_main_window;
static MenuLayer *s_menu_layer;
static GBitmap *save_icon;
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
      menu_cell_basic_draw(ctx, cell_layer, "Vickert", NULL, NULL);
      break;
    case 4:
      menu_cell_basic_draw(ctx, cell_layer, "Yes/No", NULL, NULL);
      break;
    case 5:
      menu_cell_basic_draw(ctx, cell_layer, "Something???", NULL, NULL);
      break;
   
    default:
      break;
  }
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return PBL_IF_ROUND_ELSE(
    menu_layer_is_index_selected(menu_layer, cell_index) ?
      MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT : MENU_CELL_ROUND_UNFOCUSED_TALL_CELL_HEIGHT,
    CHECKBOX_WINDOW_CELL_HEIGHT);
}



static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
  //    checkbox_window_push();
      break;
    case 1:
 radio_button_window_push();
      break;
    case 2:
 //     dialog_message_window_push();
      break;
    case 3:
  //    list_message_window_push();
      break;
    case 4:
  //    radio_button_window_push();
      break;
    case 5:
		//
      break;
 
    default:
      break;
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
save_icon = gbitmap_create_with_resource(RESOURCE_ID_HEHE);
  s_menu_layer = menu_layer_create(bounds);
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
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}