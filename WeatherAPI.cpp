//
// Created by llyfr on 3/27/23.
//

#include "WeatherAPI.h"
#include <utility>

// Creates new API object with its own Key
// We are initializing cords to fake information in case user forget to set it up
API::WeatherAPI::WeatherAPI(std::string key): key(std::move(key)), cords(0, 0) {}

API::WeatherAPI::WeatherAPI(std::string key, Cords cords, std::string units) : key(std::move(key)), cords(cords), units(std::move(units)) {}


// Would be better to just add to forecast, load data, or initialize it with it (reduces dependency)
Forecast API::WeatherAPI::getForecast() {

    std::string url = "https://api.openweathermap.org/data/3.0/onecall?lat=" + std::to_string(this->cords.lat) + "&lon=" + std::to_string(this->cords.lon) + "&units=" + units + "&appid=" + this->key;


    std::cout << url << std::endl;

    auto data = API::makeAPIcall(url);


    // TODO check for errors in reply and return false if any is detected

    return Forecast(data);
}

void API::WeatherAPI::setLocation(Cords loc) {
    this->cords = loc;
}

Cords API::WeatherAPI::getLocation() {
    return this->cords;
}


