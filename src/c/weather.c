#include <pebble.h>
#include "weather.h"
#include "messaging.h"

WeatherInfo Weather_weatherInfo;

GDrawCommandImage* Weather_currentWeatherIcon;
GDrawCommandImage* Weather_forecastWeatherIcon;

uint32_t getConditionIcon(WeatherCondition conditionCode) {
  #ifndef PBL_PLATFORM_APLITE
  uint32_t iconToLoad;

  switch(conditionCode) {
    case CLEAR_DAY:
      iconToLoad = RESOURCE_ID_WEATHER_CLEAR_DAY;
      break;
    case CLEAR_NIGHT:
      iconToLoad = RESOURCE_ID_WEATHER_CLEAR_NIGHT;
      break;
    case CLOUDY_DAY:
      iconToLoad = RESOURCE_ID_WEATHER_CLOUDY;
      break;
    case HEAVY_RAIN:
      iconToLoad = RESOURCE_ID_WEATHER_HEAVY_RAIN;
      break;
    case HEAVY_SNOW:
      iconToLoad = RESOURCE_ID_WEATHER_HEAVY_SNOW;
      break;
    case LIGHT_RAIN:
      iconToLoad = RESOURCE_ID_WEATHER_LIGHT_RAIN;
      break;
    case LIGHT_SNOW:
      iconToLoad = RESOURCE_ID_WEATHER_LIGHT_SNOW;
      break;
    case PARTLY_CLOUDY_NIGHT:
      iconToLoad = RESOURCE_ID_WEATHER_PARTLY_CLOUDY_NIGHT;
      break;
    case PARTLY_CLOUDY:
      iconToLoad = RESOURCE_ID_WEATHER_PARTLY_CLOUDY;
      break;
    case RAINING_AND_SNOWING:
      iconToLoad = RESOURCE_ID_WEATHER_RAINING_AND_SNOWING;
      break;
    case THUNDERSTORM:
      iconToLoad = RESOURCE_ID_WEATHER_THUNDERSTORM;
      break;
    default:
      iconToLoad = RESOURCE_ID_WEATHER_GENERIC;
      break;
  }

  return iconToLoad;
  #else
  return 0;
  #endif
}

void Weather_setCurrentCondition(int conditionCode) {

  uint32_t currentWeatherIcon = getConditionIcon(conditionCode);

  // ok, now load the new icon:
  #ifndef PBL_PLATFORM_APLITE
  gdraw_command_image_destroy(Weather_currentWeatherIcon);
  Weather_currentWeatherIcon = gdraw_command_image_create_with_resource(currentWeatherIcon);
  #endif

  Weather_weatherInfo.currentIconResourceID = currentWeatherIcon;
}

void Weather_setForecastCondition(int conditionCode) {
  uint32_t forecastWeatherIcon = getConditionIcon(conditionCode);

  #ifndef PBL_PLATFORM_APLITE
  gdraw_command_image_destroy(Weather_forecastWeatherIcon);
  Weather_forecastWeatherIcon = gdraw_command_image_create_with_resource(forecastWeatherIcon);
  #endif

  Weather_weatherInfo.forecastIconResourceID = forecastWeatherIcon;
}

static void request_weather_callback(void *data) {
  messaging_requestNewWeatherData();
}

void Weather_init() {
  // initialize weather with default values
  Weather_weatherInfo.currentTemp = INT32_MIN;
  Weather_weatherInfo.currentUVIndex = INT32_MIN;
  Weather_weatherInfo.todaysHighTemp = INT32_MIN;
  Weather_weatherInfo.todaysLowTemp = INT32_MIN;
  Weather_weatherInfo.last_updated = 0;

  // if possible, load weather data from persistent storage
  if (persist_exists(WEATHER_PERSIST_KEY)) {
    WeatherInfo storedWeather;
    persist_read_data(WEATHER_PERSIST_KEY, &storedWeather, sizeof(storedWeather));
    memcpy(&Weather_weatherInfo, &storedWeather, sizeof(storedWeather));
  }

  // is the weather data stale?
  if (time(NULL) - Weather_weatherInfo.last_updated > 30 * 60) {
    app_timer_register(5000, request_weather_callback, NULL);
  }
  
  // load icon if needed
  #ifndef PBL_PLATFORM_APLITE
  if (Weather_weatherInfo.currentIconResourceID > 0) {
    Weather_currentWeatherIcon = gdraw_command_image_create_with_resource(Weather_weatherInfo.currentIconResourceID);
  } else {
    Weather_currentWeatherIcon = NULL;
  }
  
  if (Weather_weatherInfo.forecastIconResourceID > 0) {
    Weather_forecastWeatherIcon = gdraw_command_image_create_with_resource(Weather_weatherInfo.forecastIconResourceID);
  } else {
    Weather_forecastWeatherIcon = NULL;
  }
  #endif
}

void Weather_saveData() {
  persist_write_data(WEATHER_PERSIST_KEY, &Weather_weatherInfo, sizeof(WeatherInfo));
}

void Weather_deinit() {
  // save weather data to persistent storage
  Weather_saveData();

  // free memory
  #ifndef PBL_PLATFORM_APLITE
  gdraw_command_image_destroy(Weather_currentWeatherIcon);
  gdraw_command_image_destroy(Weather_forecastWeatherIcon);
  #endif
}
