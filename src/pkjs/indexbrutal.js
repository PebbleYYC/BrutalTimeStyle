var Clay = require('pebble-clay');
var weather = require('./weather');

var vibs = [
  { "value": 0, "label": "None" },
  { "value": 1, "label": "Short" },
  { "value": 2, "label": "Long" },
  { "value": 3, "label": "Double" }
]

new Clay([
  {
    "type": "heading",
    "defaultValue": "BRUTAL"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Clock"
      },
      {
        "type": "color",
        "messageKey": "SettingColorTime",
        "label": "Time Color",
        "defaultValue": "000000"
      },
      {
        "type": "color",
        "messageKey": "SettingColorBG",
        "label": "Background Color",
        "defaultValue": "FFFFFF"
      },
      {
        "type": "toggle",
        "messageKey": "SettingShowLeadingZero",
        "label": "Show Leading Zero for Hours",
        "defaultValue": false
      },
      {
        "type": "select",
        "messageKey": "SettingClockFontId",
        "label": "Clock Font",
        "defaultValue": 0,
        "options": [
          { "label": "Default", "value": 0 },
          { "label": "Leco", "value": 1 },
          { "label": "Bold", "value": 2 },
          { "label": "Bold Hours", "value": 3 },
          { "label": "Bold Minutes", "value": 4 }
        ]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Sidebar"
      },
      {
        "type": "color",
        "messageKey": "SettingColorSidebar",
        "label": "Sidebar Background Color",
        "defaultValue": "000000"
      },
      {
        "type": "color",
        "messageKey": "SettingSidebarTextColor",
        "label": "Sidebar Text Color",
        "defaultValue": "FFFFFF"
      },
      {
        "type": "select",
        "messageKey": "SettingSidebarOnLeft",
        "label": "Sidebar Position",
        "defaultValue": 0,
        "options": [
          { "label": "Right", "value": 0 },
          { "label": "Left", "value": 1 }
        ]
      },
      {
        "type": "toggle",
        "messageKey": "SettingUseLargeFonts",
        "label": "Use Large Fonts in Sidebar",
        "defaultValue": false
      },
      {
        "type": "select",
        "messageKey": "SettingWidget0ID",
        "label": "Widget 1",
        "defaultValue": 1,
        "options": [
          { "label": "Empty", "value": 0 },
          { "label": "Date", "value": 1 },
          { "label": "Battery Meter", "value": 2 },
          { "label": "Current Weather", "value": 7 },
          { "label": "Weather Forecast", "value": 8 },
          { "label": "Bluetooth Disconnect", "value": 3 },
          { "label": "Week Number", "value": 9 },
          { "label": "Seconds", "value": 5 },
          { "label": "Alternate Time Zone", "value": 6 },
          { "label": "UV Index", "value": 13 },
          { "label": "Step Counter", "value": 10 },
          { "label": "Sleep Timer", "value": 11 },
          { "label": "Heart Rate", "value": 12 },
          { "label": "Swatch .beat", "value": 14 }
        ]
      },
      {
        "type": "select",
        "messageKey": "SettingWidget1ID",
        "label": "Widget 2",
        "defaultValue": 7,
        "options": [
          { "label": "Empty", "value": 0 },
          { "label": "Date", "value": 1 },
          { "label": "Battery Meter", "value": 2 },
          { "label": "Current Weather", "value": 7 },
          { "label": "Weather Forecast", "value": 8 },
          { "label": "Bluetooth Disconnect", "value": 3 },
          { "label": "Week Number", "value": 9 },
          { "label": "Seconds", "value": 5 },
          { "label": "Alternate Time Zone", "value": 6 },
          { "label": "UV Index", "value": 13 },
          { "label": "Step Counter", "value": 10 },
          { "label": "Sleep Timer", "value": 11 },
          { "label": "Heart Rate", "value": 12 },
          { "label": "Swatch .beat", "value": 14 }
        ]
      },
      {
        "type": "select",
        "messageKey": "SettingWidget2ID",
        "label": "Widget 3",
        "defaultValue": 10,
        "options": [
          { "label": "Empty", "value": 0 },
          { "label": "Date", "value": 1 },
          { "label": "Battery Meter", "value": 2 },
          { "label": "Current Weather", "value": 7 },
          { "label": "Weather Forecast", "value": 8 },
          { "label": "Bluetooth Disconnect", "value": 3 },
          { "label": "Week Number", "value": 9 },
          { "label": "Seconds", "value": 5 },
          { "label": "Alternate Time Zone", "value": 6 },
          { "label": "UV Index", "value": 13 },
          { "label": "Step Counter", "value": 10 },
          { "label": "Sleep Timer", "value": 11 },
          { "label": "Heart Rate", "value": 12 },
          { "label": "Swatch .beat", "value": 14 }
        ]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Weather"
      },
      {
        "type": "select",
        "messageKey": "SettingUseMetric",
        "label": "Temperature Units",
        "defaultValue": 0,
        "options": [
          { "label": "Fahrenheit", "value": 0 },
          { "label": "Celsius", "value": 1 }
        ]
      },
      {
        "type": "input",
        "label": "Weather Location",
        "description": "Enter a city name or a latitude,longitude pair.",
        "attributes": {
          "id": "weather_loc"
        }
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Vibrations"
      },
      {
        "type": "select",
        "messageKey": "VIBEBTOFF",
        "label": "Bluetooth disconnected",
        "defaultValue": 0,
        "options": vibs
      },
      {
        "type": "select",
        "messageKey": "VIBEBTON",
        "label": "Bluetooth connected",
        "defaultValue": 0,
        "options": vibs
      },
      {
        "type": "select",
        "messageKey": "VIBEEACHHOUR",
        "label": "Hourly",
        "defaultValue": 0,
        "options": vibs
      },
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Other"
      },
      {
        "type": "toggle",
        "messageKey": "PADH",
        "label": "Avoid drawing leading 0 in hours",
        "defaultValue": false
      },
      {
        "type": "slider",
        "messageKey": "SHADOW",
        "defaultValue": 16,
        "label": "Shadow strength",
        "description": "Dithering for numbers shadow. Some values works better than other because they give more uniform look, values like 4, 8, 16.  Use value of 0 to disable.",
        "min": 0,
        "max": 128,
        "step": 4
      },
      {
        "messageKey": "SECONDS",
        "type": "select",
        "label": "Seconds",
	"description": "First option disables seconds update; time, side and bottom texts are updated only each minute. Second option enables updates each second for side and bottom texts, use %S in their format to show seconds. Every other option enables seconds update after Tap (whist shake) for some amount of time to avoid draining battery.",
	// NOTE(Irek): Select input always return value as a string.
	// I have to do atoi() conversion in main code anyway.
        "defaultValue": "0",
        "options": [
          { "value": "0",  "label": "Disabled" },
          { "value": "-1", "label": "Enable" },
          { "value": "5",  "label": "5 seconds" },
          { "value": "10", "label": "10 seconds" },
          { "value": "15", "label": "15 seconds" },
          { "value": "30", "label": "30 seconds" },
          { "value": "60", "label": "1 min" },
          { "value": "300", "label": "5 min" }
        ]
      },
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save"
  }
], function () {
  this.on(this.EVENTS.AFTER_BUILD, function () {
  }.bind(this))
})

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log('JS component is now READY');

    // if it has never been started, set the weather to enabled
    if(window.localStorage.getItem('disable_weather') === null) {
      window.localStorage.setItem('disable_weather', 'no');
    }

    console.log('the wdisabled value is: "' + window.localStorage.getItem('disable_weather') + '"');
    // if applicable, get the weather data
    if(window.localStorage.getItem('disable_weather') != 'yes') {
      weather.updateWeather();
    }
  }
);