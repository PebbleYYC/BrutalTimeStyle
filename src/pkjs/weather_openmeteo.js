var weatherCommon = require('./weather');

module.exports.getWeatherFromCoords = getWeatherFromCoords;

function getWeatherFromCoords(pos) {
  console.log('getting that weather');
  var lat = pos.coords.latitude;
  var lon = pos.coords.longitude;

  // Request specific current weather values instead of a huge hourly forecast
  var url = 'https://api.open-meteo.com/v1/forecast?latitude=' +
    lat + '&longitude=' + lon +
    '&current=temperature_2m,weathercode,is_day,uv_index' +
    '&daily=temperature_2m_max,temperature_2m_min,weathercode' +
    '&timezone=auto';

  console.log(url);

  getAndSendWeather(url);
}

function getAndSendWeather(url) {
  weatherCommon.xhrRequest(url, 'GET', function(responseText) {
    var json = JSON.parse(responseText);

    // Handle current weather
    var temperature = Math.round(json.current.temperature_2m);
    var conditionCode = json.current.weathercode;
    var isNight = json.current.is_day !== 1;
    var uvIndex = Math.round(json.current.uv_index);

    console.log('Current temperature is ' + temperature);
    console.log('Current condition code is ' + conditionCode);

    var iconToLoad = getIconForWeatherCode(conditionCode, isNight);

    // Handle forecast
    var forecastHigh = Math.round(json.daily.temperature_2m_max[0]);
    var forecastLow = Math.round(json.daily.temperature_2m_min[0]);
    var forecastCode = json.daily.weathercode[0];

    console.log('Forecast high/low: ' + forecastHigh + '/' + forecastLow);
    console.log('Forecast condition code: ' + forecastCode);
    console.log('UV Index: ' + uvIndex);
    console.log('Is Night ' + isNight);

    var forecastIcon = getIconForWeatherCode(forecastCode);

    var dictionary = {
      'WeatherTemperature': temperature,
      'WeatherCondition': iconToLoad,
      'WeatherForecastHighTemp': forecastHigh,
      'WeatherForecastLowTemp': forecastLow,
      'WeatherForecastCondition': forecastIcon,
      'WeatherUVIndex': uvIndex
    };

    console.log(JSON.stringify(dictionary));

    weatherCommon.sendWeatherToPebble(dictionary);
  });
}

function getIconForWeatherCode(code, isNight) {
  switch (code) {
    case 0:
        return isNight ? weatherCommon.icons.CLEAR_NIGHT : weatherCommon.icons.CLEAR_DAY;
    case 1:
    case 2:
        return isNight ? weatherCommon.icons.PARTLY_CLOUDY_NIGHT : weatherCommon.icons.PARTLY_CLOUDY;
    case 3:
    case 45:
    case 48:
        return weatherCommon.icons.CLOUDY_DAY;
    case 51:
    case 53:
    case 55:
    case 61:
    case 80:
        return weatherCommon.icons.LIGHT_RAIN;
    case 63:
    case 65:
    case 81:
    case 82:
        return weatherCommon.icons.HEAVY_RAIN;
    case 56:
    case 57:
    case 66:
    case 67:
        return weatherCommon.icons.RAINING_AND_SNOWING;
    case 71:
    case 77:
    case 85:
        return weatherCommon.icons.LIGHT_SNOW;
    case 73:
    case 75:
    case 86:
        return weatherCommon.icons.HEAVY_SNOW;
    case 95:
    case 96:
    case 99:
        return weatherCommon.icons.THUNDERSTORM;
    default:
        return weatherCommon.icons.WEATHER_GENERIC;
  }
}
