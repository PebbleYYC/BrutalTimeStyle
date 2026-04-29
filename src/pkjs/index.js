var Clay = require('pebble-clay');
var clayConfig = require('./config.json');
var clay = new Clay(clayConfig);

var weather = require('./weather');

var CONFIG_VERSION = 9;
// var BASE_CONFIG_URL = 'http://localhost:4000/';
var BASE_CONFIG_URL = 'http://freakified.github.io/TimeStylePebble/';

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

// Listen for incoming messages
// when one is received, we simply assume that it is a request for new weather data
Pebble.addEventListener('appmessage',
  function(msg) {
    console.log('Recieved message: ' + JSON.stringify(msg.payload));

    // in the case of recieving this, we assume the watch does, in fact, need weather data
    window.localStorage.setItem('disable_weather', 'no');
    weather.updateWeather();
  }
);