#include <pebble.h>
#include "checkbox_window.h"
#include "dialog_message_window.h"
#include "save_routine.h"
static Window *s_main_window;
static MenuLayer *s_menu_layer;
static Layer *primitives_layer;
static TextLayer *s_list_message_layer;
static GBitmap *s_tick_black_bitmap, *s_tick_white_bitmap, *s_shade_bitmap;

static char menu_lvl;
static bool s_selections[CHECKBOX_WINDOW_NUM_ROWS];

static bool s_shade_selections[CHECKBOX_WINDOW_NUM_ROWS-1];
static bool none_of_above = false;
static int highlight_element=2;

static char* titles[]={"1",
											 "2",
											 "3",
											 "4",
											 "Żaden z pow"};
											 
static char* subtitles[] = {
	"first one",
	"second",
	"third",
	"fourth",
	"Żaden z powyższych",
	"SUBMIT"
		
};
static void update_text(int index) {
	static char s_puff[24];
		snprintf(s_puff, sizeof(s_puff), subtitles[index]);
	text_layer_set_text(s_list_message_layer, s_puff);
	
} 

static void get_selected_index(MenuLayer *menu_layer, MenuIndex *new_index, MenuIndex old_index, void *context){
	update_text(new_index->row);
	APP_LOG(APP_LOG_LEVEL_INFO, "selection changed.");

} 

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
	//do nothing
	return;
}

static void click_config_provider(void *context) {

		window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}



static void canvas_update_proc(Layer *this_layer, GContext *ctx) {
    
    graphics_draw_rect(ctx, GRect(1, 1, 142, 28));
}


static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
    return CHECKBOX_WINDOW_NUM_ROWS + 1;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
    
						

	if(cell_index->row == CHECKBOX_WINDOW_NUM_ROWS) {
        // Submit item
        menu_cell_basic_draw(ctx, cell_layer, "Submit", NULL, NULL);
    }
    else {
		
        // Choice item
        static char s_buff[16];
			//świeci na czerwono, ale działa
        snprintf(s_buff, sizeof(s_buff), titles[(int)cell_index->row] );
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
        GRect r = GRect(bounds.size.w - (2 * CHECKBOX_WINDOW_BOX_SIZE),
        (bounds.size.h / 2) - (CHECKBOX_WINDOW_BOX_SIZE / 2),
        CHECKBOX_WINDOW_BOX_SIZE, CHECKBOX_WINDOW_BOX_SIZE);
        graphics_draw_rect(ctx, r);
        if(s_selections[cell_index->row]) {
					// TOTOTO
            graphics_context_set_compositing_mode(ctx, GCompOpSet);
            graphics_draw_bitmap_in_rect(ctx, ptr, GRect(r.origin.x, r.origin.y - 3, bitmap_bounds.size.w, bitmap_bounds.size.h));
        }
			if(s_shade_selections[cell_index->row]) {
				
				graphics_context_set_compositing_mode(ctx, GCompOpSet);
				graphics_draw_bitmap_in_rect(ctx, s_shade_bitmap, GRect(r.origin.x, r.origin.y, bitmap_bounds.size.w, bitmap_bounds.size.h));
				
			}
		
			}
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
    return CHECKBOX_WINDOW_CELL_HEIGHT;
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
    if(cell_index->row == CHECKBOX_WINDOW_NUM_ROWS) {
        if(s_selections[0]==true||s_selections[1]==true||s_selections[2]==true||s_selections[3]==true||s_selections[4]==true) {
			// Do something with choices made
			//add statement checking if anything was chosen
					/*
        for(int i = 0; i < CHECKBOX_WINDOW_NUM_ROWS; i++) {
            APP_LOG(APP_LOG_LEVEL_INFO, "Option %d was %s", i, (s_selections[i] ? "selected" : "not selected"));
        }
				*/	
				checkbox_save_data(s_selections,menu_lvl);
        dialog_message_window_push();
        // window_stack_pop(true);
        } 
			else {
				//////////////////////////
				text_layer_set_text(s_list_message_layer, "pick something!!!");
			}
			}
		else {
        // Check/uncheck
        int row = cell_index->row;
        if(row == CHECKBOX_WINDOW_NUM_ROWS-1){ //NONE OF ABOVE CLICKED
           // APP_LOG(APP_LOG_LEVEL_INFO, "NONE OF ABOVE");
            none_of_above = !none_of_above;
					
						if(none_of_above){
						s_selections[row] = !s_selections[row];
            for(int i=0;i<CHECKBOX_WINDOW_NUM_ROWS-1;i++) {
                s_selections[i] =false;
							s_shade_selections[i]=true;
                //shade of box goes here	
            	}
            }
        if(none_of_above == false){
					
				
            s_selections[row] = !s_selections[row];
					 for(int i=0;i<CHECKBOX_WINDOW_NUM_ROWS-1;i++) {
               
							s_shade_selections[i]=!s_shade_selections[i];
            }
            //APP_LOG(APP_LOG_LEVEL_INFO, "clicked: %d", row);
        }
            
            
        }
else if(!none_of_above) {
	s_selections[row] = !s_selections[row];
	
} 
        menu_layer_reload_data(menu_layer);
    }
}

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    	s_shade_bitmap =gbitmap_create_with_resource(RESOURCE_ID_SHADE);
    s_tick_black_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TICK_BLACK);
    s_tick_white_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TICK_WHITE);
    
	
	s_list_message_layer = text_layer_create(GRect(5,0,140,28));
	text_layer_set_font(s_list_message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_list_message_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(s_list_message_layer));
	update_text(2);
	
	///bleblebelbelbelobeleblebe
    s_menu_layer = menu_layer_create(GRect(0, 30, 144, 158));
   menu_layer_set_click_config_onto_window(s_menu_layer, window);
    menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
        .get_num_rows = get_num_rows_callback,
        .draw_row = draw_row_callback,
        .get_cell_height = get_cell_height_callback,
       	.select_click = select_callback,
			.selection_will_change = get_selected_index,
    });
    	menu_layer_set_selected_index(s_menu_layer, MenuIndex(0,2), MenuRowAlignNone, true);

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
	if(s_shade_bitmap) {
		gbitmap_destroy(s_shade_bitmap);
	}
}

void checkbox_window_push(char choice) {
  menu_lvl=choice;
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