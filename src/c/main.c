#include <pebble.h>
#include <locale.h>
#include "settings.h"
#include "messaging.h"
#include "sidebar.h"
#include "brutal_clock.h"

static Window *main_window;

static void reconfigure_watchface() {
  GRect unobstructed_bounds = layer_get_unobstructed_bounds(window_get_root_layer(main_window));
  
  // Update background color
  window_set_background_color(main_window, settings.timeBgColor);

  // Update modules
  Sidebar_update_layout(unobstructed_bounds);
  brutal_clock_update_layout(unobstructed_bounds);
}

static void unobstructed_area_did_change_handler(void *context) {
  reconfigure_watchface();
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  Sidebar_updateTime(tick_time);
  brutal_clock_tick(tick_time, units_changed);
  Sidebar_redraw();
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  
  window_set_background_color(main_window, settings.timeBgColor);

  Sidebar_init(window);
  brutal_clock_load(window_layer);

  unobstructed_area_service_subscribe((UnobstructedAreaHandlers) {
    .did_change = unobstructed_area_did_change_handler
  }, NULL);
  
  reconfigure_watchface();
}

static void window_unload(Window *window) {
  unobstructed_area_service_unsubscribe();
  brutal_clock_unload();
  Sidebar_deinit();
}

static void init() {
  main_window = window_create();

  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  setlocale(LC_ALL, "");

  Settings_init();
  messaging_init(reconfigure_watchface);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  window_stack_push(main_window, true);
  
  // Update the time right away
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  tick_handler(t, MINUTE_UNIT);
}

static void deinit() {
  Settings_deinit();
  window_destroy(main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}