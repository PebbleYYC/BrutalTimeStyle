#pragma once
#include <pebble.h>

void brutal_clock_load(Layer *root_layer);
void brutal_clock_unload();
void brutal_clock_tick(struct tm *_time, TimeUnits change);
void brutal_clock_update_settings();
void brutal_clock_update_layout(GRect new_bounds);