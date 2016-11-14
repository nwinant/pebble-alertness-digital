#pragma once
#include <pebble.h>
#include "configuration.h"

void init_display(Window *new_s_main_window);

void update_display_config(void);

void refresh_display_layout(void);

void refresh_display_data(struct tm *tick_time);

struct TextLayer *get_connection_layer(void);

struct TextLayer *get_battery_layer(void);
