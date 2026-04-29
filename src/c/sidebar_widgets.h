#pragma once
#include <pebble.h>

/*
 * "Compact Mode" is a global setting shared by all widgets, which determines
 * whether they should try to reduce their padding. Intended to allow larger
 * widgets to fit when vertical screen space is lacking
 */
extern bool SidebarWidgets_useCompactMode;

/*
 * A global x offset used for nudging the widgets left and right
 * Included for round support
 */
extern int SidebarWidgets_xOffset;

/*
 * The different types of sidebar widgets:
 * we'll give them numbers so that we can index them in settings
 */
typedef enum {
  EMPTY                     = 0,
  DATE                      = 1,
  BATTERY_METER             = 2,
  BLUETOOTH_DISCONNECT      = 3,
  SECONDS                   = 5,
  ALT_TIME_ZONE             = 6,
  WEATHER_CURRENT           = 7,
  WEATHER_FORECAST_TODAY    = 8,
  WEEK_NUMBER               = 9,
  STEP_COUNTER              = 10,
  SLEEP_TIMER               = 11,
  HEARTRATE                 = 12,
  WEATHER_UV_INDEX          = 13,
  BEATS                     = 14
} SidebarWidgetType;

typedef struct {
  /*
   * Returns the pixel height of the widget, taking into account all current
   * settings that would affect this, such as font size
   */
  int (*getHeight)();

  /*
   * Draws the widget using the provided graphics context
   */
  void (*draw)(GContext* ctx, int yPosition);
} SidebarWidget;

void SidebarWidgets_init();
void SidebarWidgets_deinit();
SidebarWidget getSidebarWidgetByType(SidebarWidgetType type);
void SidebarWidgets_updateFonts();
void SidebarWidgets_updateTime(struct tm* timeInfo);
