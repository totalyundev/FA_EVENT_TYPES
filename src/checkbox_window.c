#include <pebble.h>
#include "checkbox_window.h"
#include "dialog_message_window.h"

static Window *s_main_window;
static MenuLayer *s_menu_layer;
static Layer *primitives_layer;

static GBitmap *s_tick_black_bitmap, *s_tick_white_bitmap;
static bool s_selections[CHECKBOX_WINDOW_NUM_ROWS];


static void canvas_update_proc(Layer *this_layer, GContext *ctx) {

	graphics_draw_rect(ctx, GRect(1, 1, 142, 38));
}


static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return CHECKBOX_WINDOW_NUM_ROWS + 1;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
  if(cell_index->row == CHECKBOX_WINDOW_NUM_ROWS) {
    // Submit item
    menu_cell_basic_draw(ctx, cell_layer, "Submit", NULL, NULL);
  }/* 
	if(cell_index->row == CHECKBOX_WINDOW_NUM_ROWS-1) {
    // Submit item
    menu_cell_basic_draw(ctx, cell_layer, "None", NULL, NULL);
  } 
	*/
	else {
    // Choice item
    static char s_buff[16];
    snprintf(s_buff, sizeof(s_buff), "Choice %d", (int)cell_index->row);
    menu_cell_basic_draw(ctx, cell_layer, s_buff, NULL, NULL);

    // Selected?
    GBitmap *ptr = s_tick_black_bitmap;
    if(menu_cell_layer_is_highlighted(cell_layer)) {
      graphics_context_set_stroke_color(ctx, GColorWhite);
      ptr = s_tick_white_bitmap;
    }

    GRect bounds = layer_get_bounds(cell_layer);
    GRect bitmap_bounds = gbitmap_get_bounds(ptr);

    // Draw checkbox
    GRect r = GRect(
      bounds.size.w - (2 * CHECKBOX_WINDOW_BOX_SIZE),
      (bounds.size.h / 2) - (CHECKBOX_WINDOW_BOX_SIZE / 2),
      CHECKBOX_WINDOW_BOX_SIZE, CHECKBOX_WINDOW_BOX_SIZE);
    graphics_draw_rect(ctx, r);
    if(s_selections[cell_index->row]) {
      graphics_context_set_compositing_mode(ctx, GCompOpSet);
      graphics_draw_bitmap_in_rect(ctx, ptr, GRect(r.origin.x, r.origin.y - 3, bitmap_bounds.size.w, bitmap_bounds.size.h));
    }
  }
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return CHECKBOX_WINDOW_CELL_HEIGHT;
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  if(cell_index->row == CHECKBOX_WINDOW_NUM_ROWS) {
    // Do something with choices made
    for(int i = 0; i < CHECKBOX_WINDOW_NUM_ROWS; i++) {
      APP_LOG(APP_LOG_LEVEL_INFO, "Option %d was %s", i, (s_selections[i] ? "selected" : "not selected"));
    }
   		dialog_message_window_push();
		// window_stack_pop(true);
  } else {
    // Check/uncheck
    int row = cell_index->row;
    s_selections[row] = !s_selections[row];
		APP_LOG(APP_LOG_LEVEL_INFO, "clicked: %d", row);
    menu_layer_reload_data(menu_layer);
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_tick_black_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TICK_BLACK);
  s_tick_white_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TICK_WHITE);
///bleblebelbelbelobeleblebe
 s_menu_layer = menu_layer_create(GRect(0, 40, 144, 128));
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = get_num_rows_callback,
      .draw_row = draw_row_callback,
      .get_cell_height = get_cell_height_callback,
      .select_click = select_callback,
  });
		
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
	primitives_layer = layer_create(bounds);
	layer_set_update_proc(primitives_layer, canvas_update_proc);
		layer_add_child(window_layer, primitives_layer);

}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);

  gbitmap_destroy(s_tick_black_bitmap);
  gbitmap_destroy(s_tick_white_bitmap);

  window_destroy(window);
  s_main_window = NULL;
}

void checkbox_window_push() {
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}
