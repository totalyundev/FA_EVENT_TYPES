#pragma once

#include <pebble.h>

#define RADIO_BUTTON_WINDOW_NUM_ROWS     4
#define RADIO_BUTTON_WINDOW_CELL_HEIGHT  28
#define RADIO_BUTTON_WINDOW_RADIO_RADIUS 5

void radio_button_window_push(char choice);
