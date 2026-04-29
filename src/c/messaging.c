#include <pebble.h>
#include <stdlib.h>
#include "weather.h"
#include "settings.h"
#include "messaging.h"
#include "message_keys.auto.h"

void (*message_processed_callback)(void);

void messaging_requestNewWeatherData() {
  // just send an empty message for now
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_uint32(iter, 0, 0);
  app_message_outbox_send();
}

void messaging_init(void (*processed_callback)(void)) {
  // register my custom callback
  message_processed_callback = processed_callback;

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(512, 8);

  // APP_LOG(APP_LOG_LEVEL_DEBUG, "Watch messaging is started!");
  app_message_register_inbox_received(inbox_received_callback);
}

void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "AppMessage received!");
  bool weatherDataUpdated = false;

  // does this message contain current weather conditions?
  Tuple *weatherTemp_tuple = dict_find(iterator, MESSAGE_KEY_WeatherTemperature);
  if(weatherTemp_tuple != NULL) {
    Weather_weatherInfo.currentTemp = (int)weatherTemp_tuple->value->int32;
    weatherDataUpdated = true;
  }

  Tuple *weatherConditions_tuple = dict_find(iterator, MESSAGE_KEY_WeatherCondition);
  if(weatherConditions_tuple != NULL) {
    Weather_setCurrentCondition(weatherConditions_tuple->value->int32);
    weatherDataUpdated = true;
  }

  Tuple *weatherUVIndex_tuple = dict_find(iterator, MESSAGE_KEY_WeatherUVIndex);
  if(weatherUVIndex_tuple != NULL) {
    Weather_weatherInfo.currentUVIndex = (int)weatherUVIndex_tuple->value->int32;
    weatherDataUpdated = true;
  }

  Tuple *weatherForecastCondition_tuple = dict_find(iterator, MESSAGE_KEY_WeatherForecastCondition);
  if(weatherForecastCondition_tuple != NULL) {
    Weather_setForecastCondition(weatherForecastCondition_tuple->value->int32);
    weatherDataUpdated = true;
  }

  Tuple *weatherForecastHigh_tuple = dict_find(iterator, MESSAGE_KEY_WeatherForecastHighTemp);
  if(weatherForecastHigh_tuple != NULL) {
    Weather_weatherInfo.todaysHighTemp = (int)weatherForecastHigh_tuple->value->int32;
    weatherDataUpdated = true;
  }

  Tuple *weatherForecastLow_tuple = dict_find(iterator, MESSAGE_KEY_WeatherForecastLowTemp);
  if(weatherForecastLow_tuple != NULL) {
    Weather_weatherInfo.todaysLowTemp = (int)weatherForecastLow_tuple->value->int32;
    weatherDataUpdated = true;
  }

  // only save new weather if weather info was recieved
  if(weatherDataUpdated) {
    Weather_weatherInfo.last_updated = time(NULL);
    Weather_saveData();
  }

  // does this message contain new config information?
  Tuple *timeColor_tuple = dict_find(iterator, MESSAGE_KEY_SettingColorTime);
  Tuple *bgColor_tuple = dict_find(iterator, MESSAGE_KEY_SettingColorBG);
  Tuple *sidebarColor_tuple = dict_find(iterator, MESSAGE_KEY_SettingColorSidebar);
  Tuple *sidebarPos_tuple = dict_find(iterator, MESSAGE_KEY_SettingSidebarOnLeft);
  Tuple *sidebarTextColor_tuple = dict_find(iterator, MESSAGE_KEY_SettingSidebarTextColor);
  Tuple *useMetric_tuple = dict_find(iterator, MESSAGE_KEY_SettingUseMetric);
  Tuple *btVibe_tuple = dict_find(iterator, MESSAGE_KEY_SettingBluetoothVibe);
  Tuple *language_tuple = dict_find(iterator, MESSAGE_KEY_SettingLanguageID);
  Tuple *leadingZero_tuple = dict_find(iterator, MESSAGE_KEY_SettingShowLeadingZero);
  Tuple *batteryPct_tuple = dict_find(iterator, MESSAGE_KEY_SettingShowBatteryPct);
  Tuple *clockFont_tuple = dict_find(iterator, MESSAGE_KEY_SettingClockFontId);
  Tuple *hourlyVibe_tuple = dict_find(iterator, MESSAGE_KEY_SettingHourlyVibe);
  Tuple *useLargeFonts_tuple = dict_find(iterator, MESSAGE_KEY_SettingUseLargeFonts);

  Tuple *widget0Id_tuple = dict_find(iterator, MESSAGE_KEY_SettingWidget0ID);
  Tuple *widget1Id_tuple = dict_find(iterator, MESSAGE_KEY_SettingWidget1ID);
  Tuple *widget2Id_tuple = dict_find(iterator, MESSAGE_KEY_SettingWidget2ID);

  Tuple *altclockName_tuple = dict_find(iterator, MESSAGE_KEY_SettingAltClockName);
  Tuple *altclockOffset_tuple = dict_find(iterator, MESSAGE_KEY_SettingAltClockOffset);

  Tuple *decimalSeparator_tuple = dict_find(iterator, MESSAGE_KEY_SettingDecimalSep);
  Tuple *healthUseDistance_tuple = dict_find(iterator, MESSAGE_KEY_SettingHealthUseDistance);
  Tuple *healthUseRestfulSleep_tuple = dict_find(iterator, MESSAGE_KEY_SettingHealthUseRestfulSleep);

  Tuple *autobattery_tuple = dict_find(iterator, MESSAGE_KEY_SettingDisableAutobattery);

  Tuple *activateDisconnectIcon_tuple = dict_find(iterator, MESSAGE_KEY_SettingDisconnectIcon);
  Tuple *shadow_tuple = dict_find(iterator, MESSAGE_KEY_SHADOW);


  if(timeColor_tuple != NULL) {
    settings.timeColor = GColorFromHEX(timeColor_tuple->value->int32);
  }

  if(bgColor_tuple != NULL) {
    settings.timeBgColor = GColorFromHEX(bgColor_tuple->value->int32);
  }

  if(sidebarColor_tuple != NULL) {
    settings.sidebarColor = GColorFromHEX(sidebarColor_tuple->value->int32);
  }

  if(sidebarTextColor_tuple != NULL) {
    // text can only be black or white, so we'll enforce that here
    settings.sidebarTextColor = GColorFromHEX(sidebarTextColor_tuple->value->int32);
  }

  if(sidebarPos_tuple != NULL) {
    settings.sidebarOnLeft = (bool)atoi(sidebarPos_tuple->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "Sidebar on Left: %d", settings.sidebarOnLeft);
  }

  if(useMetric_tuple != NULL) {
    settings.useMetric = (bool)atoi(useMetric_tuple->value->cstring);
  }

  if(btVibe_tuple != NULL) {
    settings.btVibe = (bool)btVibe_tuple->value->int8;
  }

  if(leadingZero_tuple != NULL) {
    settings.showLeadingZero = (bool)leadingZero_tuple->value->int8;
    APP_LOG(APP_LOG_LEVEL_INFO, "Show Leading Zero: %d", settings.showLeadingZero);
  }

  if(batteryPct_tuple != NULL) {
    settings.showBatteryPct = (bool)batteryPct_tuple->value->int8;
  }

  if(autobattery_tuple != NULL) {
    settings.disableAutobattery = (bool)autobattery_tuple->value->int8;
  }

  if(clockFont_tuple != NULL) {
    settings.clockFontId = atoi(clockFont_tuple->value->cstring);
  }

  if(useLargeFonts_tuple != NULL) {
    settings.useLargeFonts = (bool)useLargeFonts_tuple->value->int8;
  }

  if(hourlyVibe_tuple != NULL) {
    settings.hourlyVibe = atoi(hourlyVibe_tuple->value->cstring);
  }

  if(language_tuple != NULL) {
    settings.languageId = atoi(language_tuple->value->cstring);
  }

  if(widget0Id_tuple != NULL) {
    settings.widgets[0] = atoi(widget0Id_tuple->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "Widget 0 ID: %d", settings.widgets[0]);
  }

  if(widget1Id_tuple != NULL) {
    settings.widgets[1] = atoi(widget1Id_tuple->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "Widget 1 ID: %d", settings.widgets[1]);
  }

  if(widget2Id_tuple != NULL) {
    settings.widgets[2] = atoi(widget2Id_tuple->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "Widget 2 ID: %d", settings.widgets[2]);
  }

  if(altclockName_tuple != NULL) {
    strncpy(settings.altclockName, altclockName_tuple->value->cstring, sizeof(settings.altclockName));
  }

  if(altclockOffset_tuple != NULL) {
    settings.altclockOffset = altclockOffset_tuple->value->int8;
  }

  if(decimalSeparator_tuple != NULL) {
    settings.decimalSeparator = (char)decimalSeparator_tuple->value->int8;
  }

  if(healthUseDistance_tuple != NULL) {
    settings.healthUseDistance = (bool)healthUseDistance_tuple->value->int8;
  }

  if(healthUseRestfulSleep_tuple != NULL) {
    settings.healthUseRestfulSleep = (bool)healthUseRestfulSleep_tuple->value->int8;
  }

  if(activateDisconnectIcon_tuple != NULL) {
    settings.activateDisconnectIcon = (bool)activateDisconnectIcon_tuple->value->int8;
  }

  if(shadow_tuple != NULL) {
    settings.shadow = (uint8_t)shadow_tuple->value->int32;
  }

  // save the new settings to persistent storage
  Settings_saveToStorage();

  // notify the main screen, in case something changed
  message_processed_callback();
}

void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped! Reason: %d", reason);
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed! Reason: %d", reason);

}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  // APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}
